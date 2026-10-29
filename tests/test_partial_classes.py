"""Regression checks for partial classes: python3 tests/test_partial_classes.py [mlc]."""
import json
from pathlib import Path
import subprocess
import sys
import tempfile


def main():
    mlc = Path(sys.argv[1] if len(sys.argv) > 1 else 'build_cli/mlc').resolve()
    with tempfile.TemporaryDirectory(prefix='mlc-partial-') as tmp:
        root = Path(tmp)
        (root / 'config').mkdir()
        (root / 'additional').mkdir()
        config = {
            'source': {'configs': ['config', 'additional'], 'data': []},
            'generation': [{'language': 'cpp', 'out': 'out', 'auto_registration': False}],
        }
        (root / 'mlc.json').write_text(json.dumps(config))

        def generate(first, second):
            (root / 'config' / 'first.mlc').write_text(first)
            (root / 'additional' / 'second.mlc').write_text(second)
            return subprocess.run([str(mlc), '--config', str(root / 'mlc.json')],
                                  capture_output=True, text=True)

        first = '''class Base {}
class data/Partial<Base>:storage {
    string name
    fn void run(int value) {}
    constructor():generate
}'''
        second = '''class data/Partial {
    int extra
    fn void run(string text) {}
    fn<T> void generic(T value) {}
}
class data/Partial { fn void added() {} }'''
        for left, right in [(first, second), (second, first)]:
            result = generate(left, right)
            assert result.returncode == 0, result.stdout + result.stderr
            header = (root / 'out/data/Partial.h').read_text()
            source = (root / 'out/data/Partial.cpp').read_text()
            for text in ['public Base', 'name', 'extra', 'added(', 'generic(']:
                assert text in header, (text, header)
            assert header.count('run(') == 2, header
            assert source.count('this->name = name;') == 1, source
            assert source.count('this->extra = extra;') == 1, source

        cases = [
            ('class other/Partial {}', 'group'),
            ('class Other {} class data/Partial<Other> {}', 'parent'),
            ('enum data/Partial { value }', 'class kind'),
            ('class data/Partial { float name }', 'member: name'),
            ('class data/Partial { fn int run(int renamed) { return 0; } }', 'method: run'),
            ('class data/Partial { fn void name() {} }', 'member/method: name'),
            ('class data/Partial { constructor(string renamed) {} }', 'constructor'),
        ]
        for second, detail in cases:
            result = generate(first, second)
            assert result.returncode != 0, (second, result.stdout)
            assert detail in result.stderr, result.stderr
            assert 'first.mlc' in result.stderr and 'second.mlc' in result.stderr

        for first_method, second_method in [
            ('fn<T> void run(list<T> a) {}', 'fn<U> void run(list<U> b) {}'),
            ('fn void run(int a=1) {}', 'fn void run(int b=2) {}'),
            ('fn void run(int a) {}', 'fn void run(int:const b) {}'),
        ]:
            result = generate('class Partial { ' + first_method + ' }',
                              'class Partial { ' + second_method + ' }')
            assert result.returncode != 0 and 'method: run' in result.stderr, result.stderr

        result = generate('class Partial { fn void run(list<int> a) {} }',
                          'class Partial { fn void run(list<string> b) {} }')
        assert result.returncode == 0, result.stdout + result.stderr
        result = generate('class Partial { class Inner { int a } }',
                          'class Partial { class Inner { int b } }')
        assert result.returncode == 0, result.stdout + result.stderr
        header = (root / 'out/PartialInner.h').read_text()
        assert 'int a;' in header and 'int b;' in header, header
    print('Partial class tests passed')


if __name__ == '__main__':
    main()
