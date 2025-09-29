from mlc_tools.core.object import Object, Objects
from mlc_tools.core.function import Function
import re
from copy import deepcopy


class EcsGenerator:

    def __init__(self):
        self.mlc_model = None

    def is_based(self, cls, name):
        if cls is None:
            return False
        if cls.group == 'ecs':
            if cls.name == name:
                return True
            for c in cls.superclasses:
                if self.is_based(c, name):
                    return True
        return False

    def get_components_name(self, cls):
        # ComponentFoo -> foo
        name = cls.name.replace('Component', '')
        name_ = ''
        for i, ch in enumerate(name):
            if ch.isupper() and i > 0:
                name_ += '_'
            name_ += ch.lower()
        return name_

    def execute(self, mlc_model):
        self.mlc_model = mlc_model
        if not mlc_model.has_class('ModelEcsBase'):
            return
        model_dungeon_class = mlc_model.get_class('ModelEcsBase')

        for cls in mlc_model.classes:
            # if cls.name == 'User'
            if self.is_based(cls, 'ComponentBase') and cls.name != 'ComponentBase':
                field = self.get_components_name(cls)

                components = mlc_model.parser.create_object('list<{}*> {}'.format(cls.name, 'components_' + field))
                model_dungeon_class.members.append(components)

                components = mlc_model.parser.create_object('map<int, {}*>:private:runtime {}'.format(cls.name, 'map_components_' + field))
                model_dungeon_class.members.append(components)

                function = mlc_model.parser.create_function('function void clear_components_{}()'.format(field))
                function.parse_method_body('''
                    map_clear(this->map_components_{0});
                    list_clear(this->components_{0});
                    '''.format(field))
                model_dungeon_class.functions.append(function)

                model_dungeon_class.functions.sort(key=lambda x: x.name)
                model_dungeon_class.members.sort(key=lambda x: x.name)

        self._generate_systems_end_turn(mlc_model, 'ModelEcsBase')

        self._generate_remove_entity(mlc_model)
        self._generate_add_model_method(mlc_model)
        self._generate_remove_model_method(mlc_model)
        self._generate_get_self_from_model_method(mlc_model)
        self._generate_has_in_model(mlc_model)
        self._generate_build_map_method(mlc_model)
        # self._generate_system_skills(mlc_model, 'update')
        # self._generate_system_skills(mlc_model, 'clean')

        self._generate_model_add_component(mlc_model)
        self._generate_model_remove_component(mlc_model, True)
        self._generate_model_remove_component(mlc_model, False)
        self._generate_model_get_component(mlc_model, True)
        self._generate_model_get_component(mlc_model, False)
        self._generate_model_copy_entity_from_model(mlc_model)
        self._generate_model_get_components(mlc_model, True)
        self._generate_model_get_components(mlc_model, False)

    def modify_sources(self, mlc_model, mlc_class, header, source):
        if(mlc_class.name == 'ModelEcsBase'):
            header, source = EscAddTemplateMethods().generate(mlc_model, mlc_class, header, source)
        return (header, source)

    def get_prefedined_files(self, mlc_model):
        return EscAddTemplateMethods().get_prefedined_files(mlc_model)

    def _generate_systems_end_turn(self, mlc_model, dungeon_model_class_name):
        model_dungeon_class = mlc_model.get_class(dungeon_model_class_name)
        function = None
        for f in model_dungeon_class.functions:
            if f.name == 'update_systems':
                function = f
                break
        if function is None:
            print(f'{dungeon_model_class_name}::update_systems not founded')
            return
        systems = function.operations[0].split('\n') if len(function.operations) else []
        function.operations = []
        for i, operator in enumerate(systems):
            condition = re.findall(r'\[(!*is\w+)\]', operator)
            system = re.findall(r'\[(System\w+)\]', operator)
            if system:
                if_cond = 'if({})\n{{\n'.format(condition[0]) if condition else ''
                if_end = '\n}' if condition else ''
                clean = ''
                system = system[0]
                system_cls = mlc_model.get_class(system)
                for f in system_cls.functions:
                    if f.name == 'clean':
                        clean = '\ns{index}.clean(this);'.format(index=i)
                        break
                operator = '\n{if_cond}{system} s{index};\ns{index}.update(this, dt);{clean}{if_end}'.format(system=system, index=i, clean=clean, if_cond=if_cond, if_end=if_end)
                function.operations.append(operator)
            elif operator:
                function.operations.append(operator + ';')

    def _generate_add_model_method(self, mlc_model):
        for cls in mlc_model.classes:
            if self.is_based(cls, 'ComponentBase'):
                function = mlc_model.parser.create_function('function void add_self_to_model(ModelEcsBase* model_dungeon_class)')
                if cls.name != 'ComponentBase':
                    field = self.get_components_name(cls)
                    function.parse_method_body('model_dungeon_class->add<%s>(this);' % cls.name)
                else:
                    function.is_abstract = True
                function.is_virtual = True
                cls.functions.append(function)

    def _generate_remove_model_method(self, mlc_model):
        for cls in mlc_model.classes:
            if self.is_based(cls, 'ComponentBase'):
                function = mlc_model.parser.create_function('function void remove_self_from_model(ModelEcsBase* model_dungeon_class)')
                if cls.name != 'ComponentBase':
                    field = self.get_components_name(cls)
                    function.parse_method_body('model_dungeon_class->remove<%s>(this);' % cls.name)
                else:
                    function.is_abstract = True
                function.is_virtual = True
                cls.functions.append(function)

    def _generate_get_self_from_model_method(self, mlc_model):
        for cls in mlc_model.classes:
            if self.is_based(cls, 'ComponentBase'):
                if cls.name == 'ComponentBase':
                    continue
                function = mlc_model.parser.create_function('function ComponentBase* get_self_from_model(ModelEcsBase* model_dungeon_class, int id)')
                field = self.get_components_name(cls)
                function.parse_method_body('return model_dungeon_class->get<%s>(id);' % cls.name)
                function.is_virtual = True
                cls.functions.append(function)

    def _generate_has_in_model(self, mlc_model):
        for cls in mlc_model.classes:
            if self.is_based(cls, 'ComponentSkillBase'):
                function = mlc_model.parser.create_function('function bool has_in_model(ModelEcsBase* model_dungeon_class, int id)')
                if cls.name == 'ComponentSkillBase':
                    continue
                field = self.get_components_name(cls)
                function.parse_method_body('return model_dungeon_class->get<%s>(id) != nullptr;' % cls.name)
                function.is_virtual = True
                cls.functions.append(function)

    def _generate_build_map_method(self, mlc_model):
        model_dungeon_class = mlc_model.get_class('ModelEcsBase')
        function = mlc_model.parser.create_function('function void build_maps()')
        for cls in mlc_model.classes:
            if self.is_based(cls, 'ComponentBase'):
                if cls.name == 'ComponentBase':
                    continue;
                field = self.get_components_name(cls)
                components_name = self.get_components_name(cls)
                function.operations.append('for(auto& component : this->components_%s)' % components_name)
                function.operations.append('{')
                function.operations.append('    this->map_components_%s[component->id] = component;' % components_name)
                function.operations.append('}')
        model_dungeon_class.functions.append(function)


    def _generate_remove_entity(self, mlc_model):
        model_dungeon_class = mlc_model.get_class('ModelEcsBase')
        function = None
        for f in model_dungeon_class.functions:
            if f.name == 'remove_entity':
                function = f
                break
        if function is None:
            print('ModelEcsBase::remove_entity not founded')
            return
        for cls in mlc_model.classes:
            # if cls.name == 'User'
            if self.is_based(cls, 'ComponentBase') and cls.name != 'ComponentBase':
                field = self.get_components_name(cls)
                function.operations.append(f'''
                    if(in_map(id, this->map_components_{self.get_components_name(cls)}))
                    {{
                        auto component = this->map_components_{self.get_components_name(cls)}.at(id);
                        this->remove<{cls.name}>(component);
                    }}''')

    def _get_skill_components(self, mlc_model):
        classes = []
        for cls in mlc_model.classes:
            if self.is_based(cls, 'ComponentSkillBase') and cls.name != 'ComponentSkillBase':
                classes.append(cls)
        return classes

    def _generate_system_skills(self, mlc_model, method_name):
        skills = self._get_skill_components(mlc_model)
        system = mlc_model.get_class('SystemSkillBase')
        method = None
        for func in system.functions:
            if func.name == method_name:
                method = func
                break
        operations = method.operations
        method.operations = []
        for skill in skills:
            if len(skill.subclasses) > 0:
                continue

            field = self.get_components_name(skill)
            list_name = 'components_' + field
            for line in operations:
                method.operations.append(line.replace('@(components)', list_name))

            system.user_includes.add(skill.name)

    def _generate_model_method_save_skills(self, mlc_model):
        controller_dungeon_class = mlc_model.get_class('ControllerDungeonBase')
        save_skills_current_hero = None
        restore_hero_skill_on_change = None
        for f in controller_dungeon_class.functions:
            if f.name == 'save_skills_current_hero':
                save_skills_current_hero = f
            if f.name == 'restore_hero_skill_on_change':
                restore_hero_skill_on_change = f
            if restore_hero_skill_on_change and save_skills_current_hero:
                break
        if save_skills_current_hero is None:
            print('ControllerDungeonBase::save_skills_current_hero not found')
            return
        if restore_hero_skill_on_change is None:
            print('ControllerDungeonBase::restore_hero_skill_on_change not found')
            return

        ModelDungeonBaseChangeHeroesInfo = mlc_model.get_class('ModelDungeonBaseChangeHeroesInfo')

        save_skills_current_hero.operations.append('auto id = this->model->player_id;')
        save_skills_current_hero.operations.append('auto name = this->model->get<ComponentData>(id)->data->name;')
        restore_hero_skill_on_change.operations.append('auto name = this->model->get<ComponentData>(this->model->player_id)->data->name;')
        
        skills = self._get_skill_components(mlc_model)
        for skill in skills:
            field = self.get_components_name(skill)
            save_skills_current_hero.operations.append(f'this->model->change_heroes_info.{field}[name] = this->model->get<{skill.name}>(id);')
            
            restore_hero_skill_on_change.operations.append(f'if(in_map(name, this->model->change_heroes_info.{field}) && this->model->change_heroes_info.{field}[name])')
            restore_hero_skill_on_change.operations.append('{')
            restore_hero_skill_on_change.operations.append(f'    this->model->add<{skill.name}>(this->model->change_heroes_info.{field}[name]);')
            restore_hero_skill_on_change.operations.append('}')

            ModelDungeonBaseChangeHeroesInfo.members.append(mlc_model.parser.create_object(f'map<string, {skill.name}*> {field}'))

    def _generate_model_get_component(self, mlc_model, is_const):
        model_dungeon_class = mlc_model.get_class('ModelEcsBase')

        method = Function()
        method.name = 'get'
        method.args.append(['component_id', Objects.INT])
        method.return_type = Object()
        method.return_type.type = f'template <class T> {"const T*" if is_const else "intrusive_ptr<T>"}'
        method.is_const = is_const
        method.is_template = True
        model_dungeon_class.functions.insert(0, method)
        
        for cls in mlc_model.classes:
            if self.is_based(cls, 'ComponentBase') and cls.name != 'ComponentBase':
                field = self.get_components_name(cls)
                body = f'''template<> {f"const {cls.name}*" if is_const else f"intrusive_ptr<{cls.name}>"} ModelEcsBase::get(int component_id){" const" if is_const else ""}
                {{
                    if(in_map(component_id, this->map_components_{field}))
                    {{
                        return this->map_components_{field}.at(component_id);
                    }}
                    return nullptr;
                }}
                '''
                method.specific_implementations += body


    def _generate_model_add_component(self, mlc_model):
        model_dungeon_class = mlc_model.get_class('ModelEcsBase')

        method = Function()
        method.name = 'add'
        method.args.append(['component', mlc_model.parser.create_object('intrusive_ptr<T>')])
        method.args.append(['component_id', Object()])
        method.args[-1][1].type = 'int'
        method.args[-1][1].initial_value = '0'
        method.return_type = Object()
        method.return_type.type = f'template <class T> void'
        method.is_const = False
        method.is_template = True
        model_dungeon_class.functions.append(method)

        classes = self.get_component_classes(mlc_model)
        for cls in classes:
            field = self.get_components_name(cls)
            body = f'''template<> void ModelEcsBase::add(intrusive_ptr<{cls.name}> component, int component_id)
            {{
            '''
            for parent in cls.superclasses:
                if parent.name != 'ComponentBase':
                    body += f'this->add<{parent.name}>(intrusive_ptr<{parent.name}>(component), component_id);\n'
            body += f'''
                assert(component->id == 0 || component->id == component_id || component_id == 0);
                if(component_id != 0)
                {{
                    component->id = component_id;
                }}
                assert(component->id > 0);
                if(in_map(component->id, this->map_components_{field}))
                {{
                    list_remove(this->components_{field}, this->map_components_{field}.at(component->id));
                }}

                auto iter = std::lower_bound(this->components_{field}.begin(), this->components_{field}.end(), component, [](const auto& a, const auto& b){{return a->id < b->id;}});
                this->components_{field}.insert(iter, component);
                assert(std::is_sorted(this->components_{field}.begin(), this->components_{field}.end(), [](const auto& l, const auto& r){{return l->id < r->id;}}));

                this->map_components_{field}[component->id] = component;
            }}
            '''
            method.specific_implementations += body

        method = Function()
        method.name = 'add'
        method.args.append(['component', mlc_model.parser.create_object('T*')])
        method.args.append(['component_id', Object()])
        method.args[-1][1].type = 'int'
        method.args[-1][1].initial_value = '0'
        method.return_type = Object()
        method.return_type.type = f'template <class T> void'
        method.is_const = False
        method.is_template = True
        model_dungeon_class.functions.append(method)

        for cls in mlc_model.classes:
            if self.is_based(cls, 'ComponentBase') and cls.name != 'ComponentBase':
                field = self.get_components_name(cls)
                body = f'''template<> void ModelEcsBase::add({cls.name}* component, int component_id)
                {{
                    this->add(intrusive_ptr<{cls.name}>(component), component_id);
                }}
                '''
                method.specific_implementations += body

    def get_component_classes(self, mlc_model):
        classes = []
        for cls in mlc_model.classes:
            if self.is_based(cls, 'ComponentBase') and cls.name != 'ComponentBase':
                if cls.name == 'ComponentSkillBase':
                    classes.insert(0, cls)
                else:
                    classes.append(cls)
        return classes

    def _generate_model_remove_component(self, mlc_model, use_raw_pointer):
        model_dungeon_class = mlc_model.get_class('ModelEcsBase')

        method = Function()
        method.name = 'remove'
        if use_raw_pointer:
            method.args.append(['component', mlc_model.parser.create_object('intrusive_ptr<T>')])
        else:
            method.args.append(['component', mlc_model.parser.create_object('T*')])
        method.return_type = Object()
        method.return_type.type = f'template <class T> void'
        method.is_const = False
        method.is_template = True
        model_dungeon_class.functions.insert(0, method)

        classes = self.get_component_classes(mlc_model)
        for cls in classes:
            field = self.get_components_name(cls)
            if use_raw_pointer:
                body = f'''template<> void ModelEcsBase::remove({cls.name}* component)
                {{
                    this->remove(intrusive_ptr<{cls.name}>(component));
                }}
                '''
            else:
                body = f'''template<> void ModelEcsBase::remove(intrusive_ptr<{cls.name}> component)
                {{
                '''
                for parent in cls.superclasses:
                    if parent.name != 'ComponentBase':
                        body += f'this->remove<{parent.name}>(intrusive_ptr<{parent.name}>(component));\n'
                body += f'''if(component)
                    {{
                        list_remove(this->components_{field}, component);
                        map_remove(this->map_components_{field}, component->id);
                    }}
                }}
                '''
            method.specific_implementations += body

    def _generate_model_copy_entity_from_model(self, mlc_model):
        model_dungeon_class = mlc_model.get_class('ModelEcsBase')

        method = Function()
        method.name = 'copy_entity_from_model'
        method.args.append(['model', mlc_model.parser.create_object('ModelEcsBase*')])
        method.args.append(['id', mlc_model.parser.create_object('int')])
        method.args.append(['new_id', mlc_model.parser.create_object('int')])
        method.return_type = Object()
        method.return_type.type = 'void'
        method.is_const = False
        model_dungeon_class.functions.append(method)

        for cls in mlc_model.classes:
            if self.is_based(cls, 'ComponentBase') and cls.name != 'ComponentBase':
                field = self.get_components_name(cls)
                method.operations.append(f'''auto component_{field} = model->get<{cls.name}>(id);
                    if(component_{field})
                    {{
                        auto clone = clone_object<{cls.name}>(component_{field});
                        clone->id = new_id;
                        this->add<{cls.name}>(clone, new_id);
                    }}''')

    def _generate_model_get_components(self, mlc_model, is_const):
        model_dungeon_class = mlc_model.get_class('ModelEcsBase')

        method = Function()
        method.name = 'get_components'
        method.return_type = Object()
        if is_const:
            method.return_type.type = f'template <class T> const std::vector<intrusive_ptr<T>>&'
        else:
            method.return_type.type = f'template <class T> std::vector<intrusive_ptr<T>>&'
        method.is_const = is_const
        method.is_template = True
        model_dungeon_class.functions.insert(0, method)
        
        for cls in mlc_model.classes:
            if self.is_based(cls, 'ComponentBase') and cls.name != 'ComponentBase':
                field = self.get_components_name(cls)
                body = f'''template <> {"const " if is_const else ""}std::vector<intrusive_ptr<{cls.name}>>& ModelEcsBase::get_components() {"const" if is_const else ""}
                {{
                    return this->components_{field};
                }}
                '''
                method.specific_implementations += body

        

class EscAddTemplateMethods:
    
    def __init__(self):
        pass

    def generate(self, mlc_model, mlc_class, header, source):
        assert mlc_class.name == 'ModelEcsBase'
        header = header.replace('};', f'{MODEL_ECS_TEMPLATES}\n}};')
        header = header.split('\n')
        k = 0
        for i, line in enumerate(header):
            if '#include' in line:
                k = i
        header.insert(k+1, '#include "ecs_helper.h"')
        header = '\n'.join(header)

        return header, source

    def get_prefedined_files(self, mlc_model):
        return ['ecs/ecs_helper.h', HELPER_HPP],


MODEL_ECS_TEMPLATES = '''
public:
    template <class TCompA, class TCompB>
    std::vector<std::tuple<intrusive_ptr<TCompA>, intrusive_ptr<TCompB>>> view()const
    {
        auto& cont_a = get_components<TCompA>();
        auto& cont_b = get_components<TCompB>();
        assert(std::is_sorted(std::begin(cont_a), std::end(cont_a), [](auto const& lhs, auto const& rhs){return lhs->id < rhs->id;}));
        assert(std::is_sorted(std::begin(cont_b), std::end(cont_b), [](auto const& lhs, auto const& rhs){return lhs->id < rhs->id;}));
        
        std::vector<std::tuple<intrusive_ptr<TCompA>, intrusive_ptr<TCompB>>> result;
        result.reserve(std::min(cont_a.size(), cont_b.size()));
        
        size_t i = 0, j = 0;
        auto size_a = cont_a.size();
        auto size_b = cont_b.size();
        while (i < size_a && j < size_b)
        {
            auto& comp_a = cont_a[i];
            auto& comp_b = cont_b[j];
            int ida = comp_a->id;
            int idb = comp_b->id;
            if (ida == idb)
            {
                result.emplace_back(comp_a, comp_b);
                ++i; ++j;
            }
            else if (ida < idb)
            {
                ++i;
            }
            else
            {
                ++j;
            }
        }
        return result;
    }
    
    template<typename A, typename B, typename... Rest>
    auto view_all() const
    {
        return join_all(view<A, B>(), get_components<Rest>()...);
    }
    
    template<typename A, typename Func>
    void each(Func func)
    {
        auto& components = get_components<A>();
        for(auto& comp : components)
        {
            func(comp);
        }
    }

    template<typename A, typename B, typename... Rest, typename Func>
    void each(Func func)
    {
        // запускаем внутренний имплементационный метод с индексовой последовательностью
        each_impl<A,B,Rest...>(std::index_sequence_for<A,B,Rest...>{},func);
    }
    
    template<typename A, typename B, typename... Rest, typename Cond, typename Func>
    void each_if(Cond cond, Func func)
    {
        // запускаем внутренний имплементационный метод с индексовой последовательностью
        each_if_impl<A,B,Rest...>(std::index_sequence_for<A,B,Rest...>{},func, cond);
    }
    
private:
    //  Имплементация N-way merge через tuple<references> + индексы
    template<typename A, typename B, typename... Rest, size_t... Is, typename Func>
    void each_impl(std::index_sequence<Is...>, Func func)
    {
        // 1) Собираем tuple из ссылок на наши контейнеры
        auto containers = std::tie(get_components<A>(),
                                   get_components<B>(),
                                   get_components<Rest>()...);
        constexpr size_t N = 2 + sizeof...(Rest);
        
        // 2) Индексы по каждому вектору и их размеры
        std::array<size_t, N> idxs{};
        std::array<size_t, N> sizes
        {
            std::get<Is>(containers).size()...
        };
        
        // 3) N-way merge
        while (true)
        {
            // если любой индекс вышел за пределы — закончили
            if (((idxs[Is] >= sizes[Is]) || ...)) break;
            
            // читаем все текущие id
            std::array<int, N> ids
            {
                std::get<Is>(containers)[idxs[Is]]->id...
            };
            
            // найдём максимальный id
            int max_id = ids[0];
            for (size_t k = 1; k < N; ++k)
            {
                if (ids[k] > max_id) max_id = ids[k];
            }
            
            // если все id совпадают — вызовем func и сдвинем все индексы
            if (((ids[Is] == max_id) && ...))
            {
                func(std::get<Is>(containers)[idxs[Is]]...);
                (++idxs[Is], ...);
            }
            else
            {
                // иначе продвигаем лишь те указатели, у которых id < max_id
                for (size_t k = 0; k < N; ++k)
                {
                    if (ids[k] < max_id)
                    {
                        ++idxs[k];
                    }
                }
            }
        }
    }
    //  Имплементация N-way merge через tuple<references> + индексы
    template<typename A, typename B, typename... Rest, size_t... Is, typename Func, typename Cond>
    void each_if_impl(std::index_sequence<Is...>, Func func, Cond cond)
    {
        // 1) Собираем tuple из ссылок на наши контейнеры
        auto containers = std::tie(get_components<A>(),
                                   get_components<B>(),
                                   get_components<Rest>()...);
        constexpr size_t N = 2 + sizeof...(Rest);
        
        // 2) Индексы по каждому вектору и их размеры
        std::array<size_t, N> idxs{};
        std::array<size_t, N> sizes
        {
            std::get<Is>(containers).size()...
        };
        // 3) N-way merge
        while (true)
        {
            // если любой индекс вышел за пределы — закончили
            if (((idxs[Is] >= sizes[Is]) || ...)) break;
            
            // читаем все текущие id
            std::array<int, N> ids
            {
                std::get<Is>(containers)[idxs[Is]]->id...
            };
            
            // найдём максимальный id
            int max_id = ids[0];
            for (size_t k = 1; k < N; ++k)
            {
                if (ids[k] > max_id) max_id = ids[k];
            }
            
            // если все id совпадают — вызовем func и сдвинем все индексы
            if (((ids[Is] == max_id) && ...))
            {
                if(cond(std::get<Is>(containers)[idxs[Is]]...))
                    func(std::get<Is>(containers)[idxs[Is]]...);
                (++idxs[Is], ...);
            }
            else
            {
                // иначе продвигаем лишь те указатели, у которых id < max_id
                for (size_t k = 0; k < N; ++k)
                {
                    if (ids[k] < max_id)
                    {
                        ++idxs[k];
                    }
                }
            }
        }
    }
'''

HELPER_HPP = '''
#ifndef __mg_ecs_helper_h__
#define __mg_ecs_helper_h__

#include <vector>
#include <tuple>
#include "intrusive_ptr.h"
#include <algorithm>

template<typename A, typename B>
auto join_two_sorted(
    const std::vector<intrusive_ptr<A>>& va,
    const std::vector<intrusive_ptr<B>>& vb)
{
    std::vector<std::pair<intrusive_ptr<A>, intrusive_ptr<B>>> result;
    result.reserve(std::min(va.size(), vb.size()));

    size_t ia = 0, ib = 0;
    while (ia < va.size() && ib < vb.size()) {
        int idA = va[ia]->id;
        int idB = vb[ib]->id;
        if      (idA < idB) ++ia;
        else if (idB < idA) ++ib;
        else {
            result.emplace_back(va[ia], vb[ib]);
            ++ia; ++ib;
        }
    }
    return result;
}

// 2) join для вектора кортежей + ещё один отсортированный контейнер
template<typename Tuple, typename C>
auto join_tuple_vector(
    const std::vector<Tuple>& partial,
    const std::vector<intrusive_ptr<C>>& vc)
{
    using new_tuple_t = decltype(
        std::tuple_cat(
            std::declval<Tuple>(),
            std::declval<std::tuple<intrusive_ptr<C>>>()
        )
    );

    std::vector<new_tuple_t> result;
    result.reserve(std::min(partial.size(), vc.size()));

    size_t i = 0, j = 0;
    while (i < partial.size() && j < vc.size()) {
        // id берётся из первого элемента вложенного кортежа:
        int idP = std::get<0>(partial[i])->id;
        int idC = vc[j]->id;
        if      (idP < idC) ++i;
        else if (idC < idP) ++j;
        else {
            result.emplace_back(
                std::tuple_cat(partial[i], std::make_tuple(vc[j]))
            );
            ++i; ++j;
        }
    }
    return result;
}

// 3) Обобщённая join_all: перегруженная по количеству аргументов

// 3.1. Базовый случай: ровно два контейнера → pair
template<typename A, typename B>
auto join_all(
    const std::vector<intrusive_ptr<A>>& va,
    const std::vector<intrusive_ptr<B>>& vb)
{
    return join_two_sorted(va, vb);
}

// 3.2. Рекурсивный случай: частичный вектор кортежей + >2-й контейнер
template<typename Tuple, typename C, typename... Rest>
auto join_all(
    const std::vector<Tuple>&                  partial,
    const std::vector<intrusive_ptr<C>>&     vc,
    const std::vector<intrusive_ptr<Rest>>&... rest)
{
    auto next = join_tuple_vector(partial, vc);
    if constexpr(sizeof...(rest) > 0) {
        return join_all(next, rest...);
    }
    else {
        return next;
    }
}
#endif
'''
