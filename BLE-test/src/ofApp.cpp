#include "ofApp.h"
//#include "gattlib.h"

// UUIDs
uuid_t g_pos_uuid;
uuid_t g_button_uuid;
uuid_t reset_uuid;
const char* posuid = "64A70002-F691-4B93-A6F4-0968F5B648F8";
const char* butuid = "64A7000D-F691-4B93-A6F4-0968F5B648F8";
const char* resuid = "64A70008-F691-4B93-A6F4-0968F5B648F8";

#ifdef USE_DBUS
static GMainLoop *m_main_loop;
#endif


void notification_handler(const uuid_t* uuid, const uint8_t* data, size_t data_length, void* user_data) {

    //cout << "Notification Handler for uuid:" << uuid << endl;
    unsigned char* d = (unsigned char*)user_data;

    d[0] = data_length;
    for (int i = 0; i < data_length; i++) {
        //printf("%02x ", data[i]);
        d[i] = data[i];
    }
    //printf("\n");
}

void ofApp::exit()
{
    ofLogNotice() << "Exit called";
    if(bConnected) {
        ofLogNotice() << "Disconnecting";
        gattlib_notification_stop(connection, &g_pos_uuid);
        gattlib_notification_stop(connection, &g_button_uuid);
        gattlib_disconnect(connection);

        #ifdef USE_DBUS
        g_main_loop_unref(m_main_loop);
        #endif
    }
}

//--------------------------------------------------------------
void ofApp::setup(){
    int ret;
    bConnected = false;
    const char* deviceMAC = "eb:6c:09:bb:f6:6b";

    ofLogNotice() << "Trying to connect...";
    connection = gattlib_connect(NULL, deviceMAC, GATTLIB_CONNECTION_OPTIONS_LEGACY_DEFAULT);
    if (connection == NULL) {
        ofLogError() << "Fail to connect to the bluetooth device.";
        std::exit(1);
    }
    ofLogNotice() <<"Connected to device";
    bConnected = true;  


    // reset quaternions
    //string v = "04";
    //long int value_data = strtol(v.c_str(),NULL,16);
    short value_data = 05;
    cout << "Value data: " << value_data << endl;
    gattlib_string_to_uuid(resuid, sizeof(resuid), &reset_uuid);
    uint16_t handle = 0x001b;
    ret = gattlib_write_char_by_handle(connection, handle, &value_data, sizeof(value_data));
    if (ret != GATTLIB_SUCCESS) {
        ofLogError() << "Unsuccessful write";

        if (ret == GATTLIB_NOT_FOUND) {
            fprintf(stderr, "Could not find GATT Characteristic with UUID %s. "
                "You might call the program with '--gatt-discovery'.\n", resuid);
        } else {
            fprintf(stderr, "Error while writing GATT Characteristic with UUID %s (ret:%d)\n",
                resuid, ret);
        }
    }



//    gattlib_context_t* conn_context = connection->context;
//    GAttrib *attrib = conn_context->attrib;
//    uint8_t *value;
//    size_t len;
//    int opt_handle = 0x001b;
//    unsigned char* opt_value = {'0','7'};

//    len = gatt_attr_data_from_string(opt_value, &value);
//    if (len == 0) {
//        ofLogError() << "Invalid reset value";
//    }

//    gatt_write_cmd(attrib, opt_handle, value, len, nullptr, value);

//    g_free(value);




    gattlib_register_notification(connection, notification_handler, data);

    // start position notifications
    gattlib_string_to_uuid(posuid, sizeof(posuid), &g_pos_uuid);
    ret = gattlib_notification_start(connection, &g_pos_uuid);
    if (ret) {
        fprintf(stderr, "Fail to start notification.\n");
        gattlib_disconnect(connection);
        std::exit(1);
    }

    //start button notifications
    gattlib_string_to_uuid(butuid, sizeof(butuid), &g_button_uuid);
    ret = gattlib_notification_start(connection, &g_button_uuid);
    if (ret) {
        fprintf(stderr, "Fail to start notification.\n");
        gattlib_disconnect(connection);
        std::exit(1);
    }

    m_main_loop = g_main_loop_new(NULL, 0);
    g_main_loop_run(m_main_loop);

    //signal(SIGINT, on_user_abort);
}

//--------------------------------------------------------------
void ofApp::update(){


}

//--------------------------------------------------------------
void ofApp::draw(){
//y = numpy.int16(numpy.uint16(int.from_bytes(data[0:2], byteorder='little')))
//x = -1 * numpy.int16(numpy.uint16(int.from_bytes(data[2:4], byteorder='little')))
//w = -1 * numpy.int16(numpy.uint16(int.from_bytes(data[4:6], byteorder='little')))
//z = numpy.int16(numpy.uint16(int.from_bytes(data[6:8], byteorder='little')))

   short y = (data[1] << 8) | (data[0]);
   short x = -((data[3] << 8) | (data[2]));

    ofDrawBitmapString("x: "+ofToString(x)+" y: "+ofToString(y),20,20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    g_main_loop_quit(m_main_loop);
    std::exit(1);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
