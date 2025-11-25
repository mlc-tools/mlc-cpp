import os.path as fs
import sys


FORMAT = sys.argv[1] if len(sys.argv) > 1 else 'xml'

if FORMAT == 'xml':
    try:
        from generated_py.xml.AllTests import AllTests
        from generated_py.xml.DataStorage import DataStorage
        from generated_py.xml.RunAllTests import RunAllTests
        from generated_py.xml.Logger import Logger
    except:
        from generated_py.xml.mg import *
else:
    try:
        from generated_py.json.AllTests import AllTests
        from generated_py.json.DataStorage import DataStorage
        from generated_py.json.RunAllTests import RunAllTests
        from generated_py.json.Logger import Logger
    except:
        from generated_py.json.mg import *


class LoggerImpl(Logger):

    def message(self, message):
        print(message)


def initialize_data_storage(protocol):
    filepath = fs.abspath(fs.dirname(__file__)) + '/assets/data.' + protocol
    with open(filepath) as f:
        content = f.read()
    if protocol == 'json':
        DataStorage.shared().initialize_json(content)
    elif protocol == 'xml':
        DataStorage.shared().initialize_xml(content)


def main():
    initialize_data_storage(FORMAT)

    logger = LoggerImpl()
    result = AllTests.run(logger)

    tests = RunAllTests()
    tests.initialize(logger)
    result = result and tests.execute()
    print('Nice' if result else 'Failed')

    sys.exit(0 if result else -1)


if __name__ == '__main__':
    main()

