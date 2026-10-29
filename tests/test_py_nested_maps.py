"""Check nested map initialization and XML/JSON round trips after generation."""
import importlib
import xml.etree.ElementTree as ET

from test_py import FORMAT, initialize_data_storage


def main():
    package = 'generated_py.' + FORMAT

    def load(module, name):
        return getattr(importlib.import_module(package + '.' + module), name)

    initialize_data_storage(FORMAT)
    AllTypes = load('AllTypes', 'AllTypes')
    obj = AllTypes()
    obj.initialize()
    assert list(obj.int_unit_int[0].values()) == [0]
    assert list(obj.int_unit_int[1].values()) == [1]
    obj.int_unit_int[2]
    assert obj.int_unit_int[2] is not obj.int_unit_int[0]
    assert not AllTypes().int_unit_int
    obj.int_unit_int.clear()
    assert obj.int_unit_int[3] == {}

    Meta = load('Meta', 'Meta')
    suffix = 'Xml' if FORMAT == 'xml' else 'Json'
    Serializer = load('Serializer' + suffix, 'Serializer' + suffix)
    Deserializer = load('Deserializer' + suffix, 'Deserializer' + suffix)
    # Sets and nested containers must retain their types through serialization.
    for meta, values in [
        (Meta(set, int), {1, 2}),
        (Meta(set, Meta(set, int)), {frozenset({1, 2}), frozenset({3})}),
        (Meta(list, Meta(set, int)), [{1, 2}, {3}]),
        (Meta(dict, int, Meta(set, int)), {1: {2, 3}}),
        (Meta(list, Meta(dict, int, int)), [{1: 2}, {3: 4}]),
    ]:
        document = ET.Element('root') if FORMAT == 'xml' else {}
        Serializer(document).serialize(values, 'values')
        restored = Deserializer(document).deserialize('values', meta)
        assert restored == values, (restored, values)
        assert type(restored) is type(values)
    for depth in (2, 3):
        meta = Meta(dict, int, int)
        for _ in range(depth - 1):
            meta = Meta(dict, int, meta)
        values = meta.create_dict()
        inner = values
        for key in range(depth - 1):
            inner = inner[key]
        inner[10] = 42
        document = ET.Element('root') if FORMAT == 'xml' else {}
        Serializer(document).serialize(values, 'values')
        restored = Deserializer(document).deserialize('values', meta)
        assert restored == values
        for field in ('values', 'missing'):
            restored = Deserializer(document).deserialize(field, meta)
            restored.clear()
            inner = restored
            for key in range(depth - 1):
                inner = inner[key]
            inner[20] = 7
            assert inner == {20: 7}
        assert isinstance(values, dict)
        assert values[99] is not values[100]
    print('Nested map tests passed (' + FORMAT + ')')


if __name__ == '__main__':
    main()
