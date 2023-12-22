#pragma once
#include "DeviceDetails.h"
#include <functional>

namespace AmpProcessing {
	namespace Devices {

		/// <summary>
		/// Audio device interface for how the program can handle a device.
		/// </summary>
		class IAudioDevice {
		public:
			IAudioDevice() = default;
			virtual ~IAudioDevice() = default;

			/// <summary>
			/// Opens the device with the specific name given.
			/// </summary>
			/// <param name="deviceName">- The name of the device to use for input / output.</param>
			/// <returns>Whether the device could be opened</returns>
			virtual bool Open(const std::string& deviceName) = 0;

			/// <summary>
			/// Closes the current device.
			/// </summary>
			/// <returns>Whether the device could be closed.</returns>
			virtual bool Close() = 0;


			/// <returns>The available devices.</returns>
			virtual const std::vector<std::string> GetDeviceNames() = 0;

			/// <returns>Returns the details about the device current state.</returns>
			virtual const DeviceDetails& GetDetails() const = 0;

			/// <summary>
			/// Sets a new sample rate for the device
			/// </summary>
			/// <param name="sampleRate">- The new samplerate</param>
			/// <returns>If new sample rate could be set.</returns>
			virtual bool SetSampleRate(uint32_t sampleRate) = 0;

			/// <summary>
			/// Sets a new buffer size for the device
			/// </summary>
			/// <param name="sampleRate">- The new buffer size</param>
			/// <returns>If new buffer size could be set.</returns>
			virtual bool SetBufferSize(uint32_t bufferSize) = 0;

			/// <summary>
			/// Sets the callback for when a new sample is ready to be processed.
			/// </summary>
			/// <param name="callback">- The callback to attach.</param>
			inline void SetSampleReadyCallback(const std::function<void(std::vector<float>&)>& callback) {
				m_OnSampleReady = callback;
			};

		protected:
			/// <summary>
			/// Invokes the callback for a new sample ready.
			/// </summary>
			/// <param name="samples">- The data which is getting sent to the callback.</param>
			inline void InvokeSampleReady(std::vector<float>& samples) {
				if (!m_OnSampleReady)
					return;

				m_OnSampleReady(samples);
			}

		private:
			std::function<void(std::vector<float>&)> m_OnSampleReady;
		};
	}
}