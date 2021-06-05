

#ifndef DEVICE_DESCRIPTOR_H_
#define DEVICE_DESCRIPTOR_H_


#define MANUFACTURER_STRING_INDEX  0x01
#define PRODUCT_STRING_INDEX       0x02
#define SERIAL_NUMBER_STRING_INDEX 0x03

#define CONFIGURATION_1 0x01

#define USB_CLASS_HID      0x03
#define USB_SUBCLASS_HID   0x01
#define USB_PROTOCOL_MOUSE 0x02


#define USB_MAX_PACKET_SIZE (64U)


#include "mouse_report_descriptor.h"


const USB_Descriptor_Device_t deviceDescriptor = {
	.Header                 = {.Size = sizeof(USB_Descriptor_Device_t), .Type = DTYPE_Device},

	.USBSpecification       = 0x0110,
	.Class                  = USB_CSCP_NoDeviceClass,
	.SubClass               = USB_CSCP_NoDeviceSubclass,
	.Protocol               = USB_CSCP_NoDeviceProtocol,

	.Endpoint0Size          = USB_MAX_PACKET_SIZE,

	.VendorID               = 0x6969,
	.ProductID              = 0x4242,
	.ReleaseNumber          = 0x0001,

	.ManufacturerStrIndex   = MANUFACTURER_STRING_INDEX,
	.ProductStrIndex        = PRODUCT_STRING_INDEX,
	.SerialNumStrIndex      = NO_DESCRIPTOR,

	.NumberOfConfigurations = 1
};

IAR_ATTR_PACKED struct TASKING_ATTR_PACKED USB_Descriptor_HID{

	USB_Descriptor_Header_t Header;


	uint16_t  HIDVersionNumber;

	uint8_t  CountryCode;

	uint8_t  NumDescriptors;

	uint8_t  DescriptorType;

	uint16_t  DescriptorLength;

}ATTR_PACKED;
typedef struct USB_Descriptor_HID USB_Descriptor_HID_t;

enum USB_DescriptorMoreTypes_t {
	DTYPE_HID = 0x21,
	DTYPE_Report_HID = 0x22
};

typedef struct{
	USB_Descriptor_Configuration_Header_t    config;
	USB_Descriptor_Interface_t               winusb_interface;
	USB_Descriptor_HID_t                     winusb_hid_descriptor;
	USB_Descriptor_Endpoint_t                winusb_data_endpoint;
} USB_Mouse_Descriptor_Configuration_t;

const USB_Mouse_Descriptor_Configuration_t configurationDescriptor = {
	.config = {
		.Header                 = {.Size = sizeof(USB_Descriptor_Configuration_Header_t), .Type = DTYPE_Configuration},
		.TotalConfigurationSize = sizeof(USB_Mouse_Descriptor_Configuration_t),
		.TotalInterfaces        = 1,
		.ConfigurationNumber    = CONFIGURATION_1,
		.ConfigurationStrIndex  = NO_DESCRIPTOR,
		.ConfigAttributes       = (USB_CONFIG_ATTR_SELFPOWERED | USB_CONFIG_ATTR_REMOTEWAKEUP),
		.MaxPowerConsumption    = USB_CONFIG_POWER_MA(100)
	},

	.winusb_interface = {
		.Header                 = {.Size = sizeof(USB_Descriptor_Interface_t), .Type = DTYPE_Interface},
		.InterfaceNumber        = 0,
		.AlternateSetting       = 0,
		.TotalEndpoints         = 1,
		.Class                  = USB_CLASS_HID,
		.SubClass               = USB_SUBCLASS_HID,
		.Protocol               = USB_PROTOCOL_MOUSE,
		.InterfaceStrIndex      = NO_DESCRIPTOR
	},
	
	.winusb_hid_descriptor = {
		.Header                 = {.Size = sizeof(USB_Descriptor_HID_t), .Type = DTYPE_HID},
		.HIDVersionNumber       = 0x01,
		.CountryCode            = 0x00,
		.NumDescriptors         = 0x01,
		.DescriptorType         = DTYPE_Report_HID,
		.DescriptorLength       = sizeof(USB_HID_ReportDescriptor_Mouse2Axis),
	},

	.winusb_data_endpoint = {
		.Header                 = {.Size = sizeof(USB_Descriptor_Endpoint_t), .Type = DTYPE_Endpoint},
		.EndpointAddress        = (ENDPOINT_DIR_IN | 1),
		.Attributes             = (EP_TYPE_INTERRUPT),
		.EndpointSize           = USB_MAX_PACKET_SIZE,
		.PollingIntervalMS      = 0x0A
	}

};

const USB_Descriptor_String_t languageString = {
	.Header        = {.Size = USB_STRING_LEN(1), .Type = DTYPE_String},
	.UnicodeString = { LANGUAGE_ID_ENG }
};

const USB_Descriptor_String_t manufacturerString = {
	.Header        = {.Size = USB_STRING_LEN(13), .Type = DTYPE_String},
	.UnicodeString = {'A','n','d','r','e','i','D','o','b','o','s'}
};

const USB_Descriptor_String_t productString = {
	.Header        = {.Size = USB_STRING_LEN(22), .Type = DTYPE_String},
	.UnicodeString = {'M','o','u','s','e','S','i','e'}
};

const USB_Descriptor_String_t serialNumberString = {
	.Header        = {.Size = USB_STRING_LEN(8), .Type = DTYPE_String},
	.UnicodeString = {'2','B','A','B','C','0','D','E'}
};



uint16_t Mouse_GetDescriptor(const uint16_t w_value, const uint16_t w_index, const void** const descriptor_address){
	const uint8_t  descriptorType   = (w_value >> 8);
	const uint8_t  descriptorNumber = (w_value & 0xFF);

	const void* address = NULL;
	uint16_t    size    = NO_DESCRIPTOR;

	switch (descriptorType){
		case DTYPE_Device:
			address = &deviceDescriptor;
			size    = sizeof(USB_Descriptor_Device_t);
			break;
		case DTYPE_Configuration:
			address = &configurationDescriptor;
			size    = sizeof(USB_Mouse_Descriptor_Configuration_t);
			break;
		case DTYPE_Report_HID:
			address = &USB_HID_ReportDescriptor_Mouse2Axis;
			size    = sizeof(USB_HID_ReportDescriptor_Mouse2Axis);
			break;
		case DTYPE_String:
			switch (descriptorNumber){
				case 0x00:
					address = &languageString;
					size    = languageString.Header.Size;
					break;
				case MANUFACTURER_STRING_INDEX:
					address = &manufacturerString;
					size    = manufacturerString.Header.Size;
					break;
				case PRODUCT_STRING_INDEX:
					address = &productString;
					size    = productString.Header.Size;
					break;
				case SERIAL_NUMBER_STRING_INDEX: // can be removed
					address = &serialNumberString;
					size    = serialNumberString.Header.Size;
					break;

			}
			break;
	}

	(void)w_index;
	*descriptor_address = address;
	return size;
}


void Mouse_WakeUp(){
	int i = 1;
}


void Mouse_Suspend(){
	int i = 1;
}


void Mouse_StartOfFrame(){
	int i = 1;
}


void Mouse_SetAddress(){
	int i = 1;
}


void Mouse_Reset(){

	if (device.IsConfigured){
		device.IsConfigured = 0U;
		if (USBD_WINUSB_STATUS_SUCCESS == USBD_WINUSB_Init(USBD_WINUSB_handle)){
			USBD_WINUSB_Register_USBEvent_CB(&Mouse_event_callback);
			if (USBD_WINUSB_STATUS_SUCCESS != USBD_WINUSB_Connect()){
			}
		}
	}
}

#endif /* DEVICE_DESCRIPTOR_H_ */
