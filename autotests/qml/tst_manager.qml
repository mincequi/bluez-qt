import QtTest 1.0
import org.kde.bluezqt.fakebluez 1.0

TestCase {
    name: "Manager"

    TestUtils {
        id: utils
    }

    function initTestCase()
    {
        utils.disableDebugOutput();
    }

    function cleanupTestCase()
    {
        utils.enableDebugOutput();
    }

    function cleanup()
    {
        FakeBluez.stop();
    }

    function test_bluezNotRunning()
    {
        var manager = utils.createManager(this);
        var initResult = utils.initManager(manager);

        compare(initResult, "initFinished", "init-manager");
        verify(manager.initialized, "initialized");
        verify(!manager.operational, "operational");
        verify(!manager.bluetoothOperational, "btOperational");

        manager.destroy();
    }

    function test_bluezNotExportingInterfaces()
    {
        FakeBluez.start();
        FakeBluez.runTest("bluez-not-exporting-interfaces");

        var manager = utils.createManager(this);
        var initResult = utils.initManager(manager);

        compare(initResult, "initError", "init-manager");
        verify(!manager.initialized, "initialized");
        verify(!manager.operational, "operational");
        verify(!manager.bluetoothOperational, "btOperational");

        manager.destroy();
    }

    function test_bluezNoAdapters()
    {
        FakeBluez.start();
        FakeBluez.runTest("bluez-no-adapters");

        var manager = utils.createManager(this);
        var initResult = utils.initManager(manager);

        compare(initResult, "initFinished", "init-manager");
        verify(manager.initialized, "initialized");
        verify(manager.operational, "operational");
        verify(!manager.bluetoothOperational, "btOperational");

        manager.destroy();
    }

    SignalSpy {
        id: usableAdapterChangedSpy
        signalName: "usableAdapterChanged"
    }

    function test_usableAdapter()
    {
        FakeBluez.start();
        FakeBluez.runTest("bluez-standard");

        var adapter1path = "/org/bluez/hci0";
        var adapter1props = {
            Path: adapter1path,
            Address: "1C:E5:C3:BC:94:7E",
            Name: "TestAdapter",
            Powered: false,
            _toDBusObjectPath: [ "Path" ]
        }
        FakeBluez.runAction("devicemanager", "create-adapter", adapter1props);

        var adapter2path = "/org/bluez/hci1";
        var adapter2props = {
            Path: adapter2path,
            Address: "2E:3A:C3:BC:85:7C",
            Name: "TestAdapter2",
            Powered: false,
            _toDBusObjectPath: [ "Path" ]
        }
        FakeBluez.runAction("devicemanager", "create-adapter", adapter2props);


        var manager = utils.createManager(this);
        var initResult = utils.initManager(manager);

        compare(initResult, "initFinished", "init-manager");
        verify(manager.initialized, "initialized");
        verify(manager.operational, "operational");
        verify(!manager.bluetoothOperational, "btOperational");

        usableAdapterChangedSpy.target = manager;

        var properties = {
            Path: adapter1path,
            Name: "Powered",
            Value: true,
            _toDBusObjectPath: [ "Path" ]
        }
        FakeBluez.runAction("devicemanager", "change-adapter-property", properties);

        tryCompare(usableAdapterChangedSpy, "count", 1);
        compare(manager.usableAdapter.ubi, adapter1path);

        usableAdapterChangedSpy.clear();

        properties.Value = false;
        FakeBluez.runAction("devicemanager", "change-adapter-property", properties);

        tryCompare(usableAdapterChangedSpy, "count", 1);
        compare(manager.usableAdapter, null);

        usableAdapterChangedSpy.clear();

        properties.Path = adapter2path;
        properties.Value = true;
        FakeBluez.runAction("devicemanager", "change-adapter-property", properties);

        tryCompare(usableAdapterChangedSpy, "count", 1);
        compare(manager.usableAdapter.ubi, adapter2path);

        manager.destroy();
    }
}
