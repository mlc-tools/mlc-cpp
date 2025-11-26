//
//  SavePluginCpp.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 30.07.2025.
//

#include "SavePluginCpp.hpp"
#include "models/Class.hpp"
#include "utils/Common.hpp"
#include "Model.hpp"
#include <cassert>
#include <re2/re2.h>

void hoist_includes(std::string& code);

std::string weight_source(const SavePluginBase::FileEntry &e) {
    static std::unordered_map<std::string, std::string> prefixes = {
        {"intrusive_ptr.cpp", "0"},
        {"DataStorage.cpp", "1"},
        {"Bindings.cpp", "~"},
    };
    
    auto cls = std::get<0>(e);

    std::string w;
    auto iter = prefixes.find(std::get<1>(e));
    if(iter != prefixes.end()){
        w = iter->second;
    }
    else if (cls && !cls->parent_class_name.empty()) {
        w = "|";
    }
    if (cls)
        w += cls->name;
    return w;
};

SavePluginCpp::SavePluginCpp(Model &model, const FeatureUnityFile& feature_unity_file)
: SavePluginBase(model, feature_unity_file){
    
}

void SavePluginCpp::save_files() {
    SavePluginBase::save_files();
}

void SavePluginCpp::saveOne(){
    saveWithUnityHeaders();
    saveWithUnitySourceToOne();
}
void SavePluginCpp::saveByGroup(){
    saveWithUnityHeaders();
    saveWithUnitySourceToOneByGroups();
}
void SavePluginCpp::saveAll(){
    SavePluginBase::saveAll();
}
void SavePluginCpp::saveWithUnityHeaders(){
    for(auto& entry : model.files){
        if(ends_with(std::get<1>(entry), ".h"))
            saveFile(std::get<1>(entry), std::get<2>(entry));
    }
}
std::vector<SavePluginCpp::FileEntry*> SavePluginCpp::get_source_files_with_order(){
    
    std::vector<FileEntry*> sources;
    sources.reserve(model.files.size());
    for(auto& entry : model.files){
        if(ends_with(std::get<1>(entry), ".cpp"))
            sources.push_back(&entry);
    }
    std::sort(sources.begin(), sources.end(), [&](auto &a, auto &b) { return weight_source(*a) < weight_source(*b); });
    return sources;
}

void SavePluginCpp::saveWithUnitySourceToOne(){
    auto sources = get_source_files_with_order();
    
    std::string source;
    for(auto& entry : sources){
        source += "\n// " + std::get<1>(*entry) + "\n" + std::get<2>(*entry);
    }
    hoist_includes(source);
    
    if(source.find("#include \"Bindings_.h\"") != std::string::npos){
        replace_all(source, "#include \"Bindings_.h\"\n", "");
        replace_all(source, "// Bindings.cpp", "// Bindings.cpp\n#include \"Bindings_.h\"");
    }
    saveFile("mg.cpp", source);
}

void SavePluginCpp::saveWithUnitySourceToOneByGroups(){
    auto sources = get_source_files_with_order();
    
    std::unordered_map<std::string, std::string> groups;
    for(auto& entry : sources){
        std::string group_name;
        auto cls = std::get<0>(*entry);
        if(cls)
            group_name = cls->group;
        auto& source = groups[group_name];
        source += "\n// " + std::get<1>(*entry) + "\n" + std::get<2>(*entry);
    }
    
    auto root_source = std::move(groups[""]);
    groups.erase("");
    hoist_includes(root_source);
    if(root_source.find("#include \"Bindings_.h\"") != std::string::npos){
        replace_all(root_source, "#include \"Bindings_.h\"\n", "");
        replace_all(root_source, "// Bindings.cpp", "// Bindings.cpp\n#include \"Bindings_.h\"");
    }
    saveFile("mg.cpp", root_source);
    for(auto&& [group_name, source] : groups){
        hoist_includes(source);
        saveFile(group_name + ".cpp", source);
    }
    
}

void hoist_includes(std::string& code)
{
    // Включаем мультистрочный режим, чтобы ^ и $ работали построчно

    re2::RE2 include_re(R"((#include ([<"][^>"]*[>"]).*))");

    if (!include_re.ok())
    {
        // Можно бросить исключение или логировать ошибку
        return;
    }

    re2::StringPiece input(code);
    re2::StringPiece line_sp;   // вся строка include (группа 1)
    re2::StringPiece header_sp; // только "<...>" или "..." (группа 2)

    // Список диапазонов [start, end) в исходной строке, которые нужно вырезать
    std::vector<std::pair<size_t, size_t>> ranges;

    // Уникальные инклюды в порядке первого появления
    std::vector<std::string> order;
    std::unordered_map<std::string, std::string> header_to_line;

    while (re2::RE2::FindAndConsume(&input, include_re, &line_sp, &header_sp))
    {
        // Позиция начала совпадения относительно начала исходной строки
        size_t start = static_cast<size_t>(line_sp.data() - code.data());
        size_t end   = start + line_sp.size();

        // Захватим перевод строки(строк), чтобы удалить **всю** строку
        if (end < code.size())
        {
            if (code[end] == '\r')
            {
                ++end;
                if (end < code.size() && code[end] == '\n')
                    ++end;
            }
            else if (code[end] == '\n')
            {
                ++end;
            }
        }

        ranges.emplace_back(start, end);

        std::string header(header_sp);      // "<iostream>" или "my.h"
        std::string line(line_sp);        // оригинальная строка include

        // Уникальность по имени заголовка
        if (!header_to_line.count(header))
        {
            header_to_line[header] = line;
            order.push_back(header);
        }
    }

    // Если include-ов нет — ничего не делаем
    if (order.empty())
        return;

    // Собираем блок include-ов (в порядке первого появления, с оригинальной строкой)
    std::string includes_block;
    includes_block.reserve(64 * order.size());

    for (const auto& header : order)
    {
        includes_block += header_to_line[header];
        includes_block += '\n';
    }
    includes_block += '\n'; // пустая строка после блока include-ов

    // Удаляем все найденные include-строки из исходного текста
    std::string new_body;
    new_body.reserve(code.size());

    size_t pos = 0;
    for (const auto& r : ranges)
    {
        size_t start = r.first;
        size_t end   = r.second;
        if (pos < start)
            new_body.append(code, pos, start - pos);
        pos = end;
    }
    if (pos < code.size())
        new_body.append(code, pos, code.size() - pos);

    // Новый код: include-ы сверху, далее остальной код
    code = includes_block + new_body;
}
