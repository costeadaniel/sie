

#include <DAVE.h>

USBD_Event_CB_t Mouse_event_callback;

#include "device_descriptor.h"

void ConfigurationChanged(void);


USBD_Event_CB_t Mouse_event_callback = {
	0,
	0,
	ConfigurationChanged,
	USBD_WINUSB_Event_ControlRequest,
	Mouse_SetAddress,
	Mouse_GetDescriptor,
	Mouse_WakeUp,
	Mouse_Suspend,
	Mouse_StartOfFrame,
	Mouse_Reset
};

uint8_t buf[USB_MAX_PACKET_SIZE];

USB_Endpoint_Table_t endpointTable = {
	.Address = (ENDPOINT_DIR_IN | 1),
	.Size = USB_MAX_PACKET_SIZE,
	.Type = XMC_USBD_ENDPOINT_TYPE_INTERRUPT,
	.Banks = 1
};

void ConfigurationChanged(void){
	USBD_SetEndpointBuffer(configurationDescriptor.winusb_data_endpoint.EndpointAddress, buf, USB_MAX_PACKET_SIZE);

	Endpoint_ConfigureEndpointTable(&endpointTable, 1);

	device.IsConfigured = 1;
}

int main(void){
	DAVE_STATUS_t status;

	status = DAVE_Init();           /* Initialization of DAVE APPs  */

	USBD_WINUSB_Register_USBEvent_CB(&Mouse_event_callback);

	if(status != DAVE_STATUS_SUCCESS){
		XMC_DEBUG("DAVE APPs initialization failed\n");

		while(1U){

		}
	}

	if(USBD_WINUSB_Connect() != USBD_WINUSB_STATUS_SUCCESS){
		return -1;
	}

	while(!USBD_WINUSB_IsEnumDone()){

	}


	return 0;
}
