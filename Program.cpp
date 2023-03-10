#include <blueberry/blueberry.h>
#include <iostream>
#include <chrono>
#include <thread>

#include <gtest/gtest.h>

int main()
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}

TEST(Systemtest, FirstTest)
{
    // Create a fresh Blueberry instance we will use for all the interactions with the Blueberry SDK
    bb::BlueberryInstancePtr instance = bb::BlueberryInstance(MODULE_PATH);

    // Add device by connection string
    bb::DevicePtr device;
    device = instance.addDevice("bb://127.0.0.1");

    // Exit if no device is found
    if (!device.assigned())
    {
        std::cout << "No device simulator found." << std::endl; 
    }

    // Output the name of the added device
    std::cout << device.getDeviceInfo().getName() << std::endl;

     // Output 10 samples using reader
    using namespace std::chrono_literals;
    bb::StreamReaderPtr<double> reader = bb::StreamReader<double>(device.getSignals(false)[0]);

    // Allocate buffer for reading double samples
    double samples[100];
    for (int i = 0; i < 10; ++i)
    {
        std::this_thread::sleep_for(100ms);

        // Read up to 100 samples, storing the amount read into `count`
        bb::SizeT count = 100;
        reader.readSamples(samples, &count);
        if (count > 0)
            std::cout << samples[count - 1] << std::endl;
    }

    // Create an instance of the renderer function block
    bb::FunctionBlockPtr renderer = instance.addFunctionBlock("ref_fb_module_renderer");
    // Connect the first output signal of the device to the renderer
    renderer.getInputPorts()[0].connectSignal(device.getSignals(false)[0]);

    // Create an instance of the averager function block
    bb::FunctionBlockPtr averager = instance.addFunctionBlock("ref_fb_module_averager");
    // Connect the first output signal of the device to the averager
    averager.getInputPorts()[0].connectSignal(device.getSignals(false)[0]);
    // Connect the first output signal of the averager to the renderer
    renderer.getInputPorts()[1].connectSignal(averager.getOutputSignals()[0]);



    // Get the first channel of the device
    const bb::ChannelPtr sineChannel = device.getChannels(false)[0];

    // List the names of all properties
    for (bb::PropertyInfoPtr prop : sineChannel.enumPropertiesInfo())
    {
        std::cout << prop.getName() << std::endl;
    }

    // Set the frequency to 5 Hz
    sineChannel.setPropertyValue("Frequency", 5);
    // Set the noise amplitude to 0.75
    sineChannel.setPropertyValue("NoiseAmplitude", 0.75);

    // Modulate the signal amplitude by a step of 0.1 every 25ms.
    double amplStep = 0.1;
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
        const double ampl = sineChannel.getPropertyValue("Amplitude");
        if (9.95 < ampl || ampl < 1.05)
            amplStep *= -1;
        sineChannel.setPropertyValue("Amplitude", ampl + amplStep);
    }
}
