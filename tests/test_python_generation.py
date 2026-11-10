"""Regression tests for Python generation used by the game data tools."""
import importlib
import json
from pathlib import Path
import subprocess
import sys
import tempfile
import xml.etree.ElementTree as ET


SOURCE = r'''
enum Mode { first second }
class Value { int number = 7 }
class DataEntry:storage { string name }
class EnumDefaults { Mode mode = Mode::second }
class Sample {
    Value:static shared
    int name = 100
    int result
    constructor(int unused):cpp { result = unused; }
    constructor(int value = Mode::second):py { result = value; }
    fn int default_value(int value = Mode::second):static { return value; }
    fn int enabled(bool value=true):static { return value ? 1 : 0; }
    fn int overload() { return 3; }
    fn int overload(int value) { return overload() + value; }
    fn int run() {
        map<int, int> values;
        values[1] = 4;
        values[2] = 8;
        int sum = 0;
        for(auto& [name, value] : values)
        {
            sum += value;
        }
        for(int i=-2; i<=2; ++i)
        {
            sum += i;
        }
        for(int i=0; i>=-2; --i)
        {
            sum += i;
        }
        return sum;
    }
    fn int choose(Value* value) { return 1; }
    fn int choose(Child* value) { return 2; }
    fn int cast() { return static_cast<int>(overload(2)); }
    fn int containers() {
        map<int, int> values;
        values[1] = 3;
        values[2] = 7;
        map_remove_if(values, (key, value :> auto remove = value > 4; remove));
        int result = values.empty() ? 0 : values.size();
        {
            result += 1;
        }
        return result;
    }
}
class Visited:visitor {}
class Child<Visited> {}
class FirstVisitor<IVisitorVisited> {
    int result
    fn void visit(Child* value) { result = 1; }
}
class SecondVisitor<IVisitorVisited> {
    int result
    fn void visit(Child* value) { result = 2; }
}
'''


def main():
    mlc = Path(sys.argv[1] if len(sys.argv) > 1 else 'build_cli/mlc').resolve()
    with tempfile.TemporaryDirectory(prefix='mlc-python-') as tmp:
        root = Path(tmp)
        (root / 'config').mkdir()
        (root / 'config/test.mlc').write_text(SOURCE)
        config = {
            'source': {'configs': ['config'], 'data': []},
            'generation': [{'language': 'py', 'out': 'generated',
                            'serialize_protocol': ['xml', 'json'],
                            'features': {'visitor': {}, 'data_storage': {}}}],
        }
        (root / 'mlc.json').write_text(json.dumps(config))
        subprocess.run([str(mlc), '--config', str(root / 'mlc.json')], check=True)
        sys.path.insert(0, str(root))
        for source in (root / 'generated').glob('*.py'):
            importlib.import_module('generated.' + source.stem)
        from generated.Sample import Sample
        from generated.Value import Value
        from generated.DataStorage import DataStorage
        from generated.Mode import Mode
        from generated.Child import Child
        from generated.FirstVisitor import FirstVisitor
        from generated.SecondVisitor import SecondVisitor
        from generated.EnumDefaults import EnumDefaults
        from generated.SerializerXml import SerializerXml
        from generated.SerializerJson import SerializerJson
        from generated.DeserializerXml import DeserializerXml
        from generated.DeserializerJson import DeserializerJson
        for serializer_type, deserializer_type, document in [
            (SerializerXml, DeserializerXml, ET.Element('root')),
            (SerializerJson, DeserializerJson, {}),
        ]:
            obj = EnumDefaults()
            suffix = 'xml' if serializer_type is SerializerXml else 'json'
            # Exercise the enum default even when serialization omits its value.
            getattr(obj, 'serialize_' + suffix)(serializer_type(document))
            obj.mode = Mode.first
            getattr(obj, 'serialize_' + suffix)(serializer_type(document))
            restored = EnumDefaults()
            getattr(restored, 'deserialize_' + suffix)(deserializer_type(document))
            assert restored.mode == Mode.first
        sample = Sample()
        assert sample.result == Mode.second
        assert sample.default_value() == Mode.second
        assert sample.enabled() == 1 and Sample.enabled(False) == 0
        assert sample.shared.number == 7
        assert sample.overload() == 3 and sample.overload(4) == 7
        assert sample.cast() == 5
        assert sample.containers() == 2
        assert sample.choose(Value()) == 1 and sample.choose(Child()) == 2
        DataStorage.shared().initialize_xml('<data/>')
        assert DataStorage.shared().getDataEntry('missing') is None
        assert DataStorage.shared().getDataEntry(None) is None
        assert sample.run() == 9 and sample.name == 100
        for visitor_type, expected in [(FirstVisitor, 1), (SecondVisitor, 2)]:
            visitor = visitor_type()
            Child().accept(visitor)
            assert visitor.result == expected
    print('Python generation regression tests passed')


if __name__ == '__main__':
    main()
