#include "ofMain.h"
#include "ofxPubSubOsc.h"


const int kBufferSize = 256;
const int kSampleRate = 44100;
const std::string kAddress = "127.0.0.1";
const int kPort = 8888;

class ofApp : public ofBaseApp{
    
public:
    void setup()
    {
        ofBackground(0, 0, 0);
        ofSetWindowTitle("Audio Input Volume Sender App");
        smoothed_vol = 0.f;
        scaled_vol   = 0.f;
        setInputAudioDevice();
        ofxPublishOsc(kAddress, kPort, "/volume", scaled_vol);
    }
    
    void setInputAudioDevice(){
        auto device_list = sound.getDeviceList();
        std::string device_name;
        
        for(auto i = 0; i < device_list.size(); ++i){
            if(i == device_list.size()-1){
                device_name += ofToString(device_list[i]);
            }else{
                device_name += ofToString(device_list[i]) + "\n";
            }
        }
        
        int selected_device_id = ofToInt(ofSystemTextBoxDialog(device_name));
        sound.setDeviceID(selected_device_id);
        sound.setup(this, 0, 2, kSampleRate, kBufferSize, 4);
        left.assign(kBufferSize, 0.f);
        right.assign(kBufferSize, 0.f);
    }
    
    void update(){
        
        scaled_vol = ofClamp(ofMap(smoothed_vol, 0.f, 0.17, 0.f, 1.f), 0.f, 1.f);
        //        scaled_vol = ofMap(smoothed_vol, 0.f, 0.17, 0.f, 1.f);
    }
    
    void draw(){
        ofSetColor(255,255,255);
        ofDrawBitmapString("Volume: " + ofToString(scaled_vol), 20, 40);
        ofDrawBitmapString("IP: " + kAddress, 20, 60);
        ofDrawBitmapString("Port: " + ofToString(kPort), 20, 80);
        ofDrawBitmapString("---Key info ----", 20, 100);
        ofDrawBitmapString("press 's': Stop audio", 20, 120);
        ofDrawBitmapString("press 'p': Play audio", 20, 140);
        
        drawAudioInput();
    }
    
    void keyPressed(int key){
        if(key == 's'){
            sound.stop();
        }
        if(key == 'p'){
            sound.start();
        }
    }
    
    void audioIn(float* input, int bufferSize, int nChannels)
    {
        float cur_vol = 0.f;
        int num_counted = 0;
        
        for (auto i = 0 ; i < bufferSize; ++i){
            left[i]  = input[i * 2] * 0.5;
            right[i] = input[i * 2 + 1] * 0.5;
            cur_vol += left[i] * left[i];
            cur_vol += right[i] * right[i];
            num_counted += 2;
        }
        cur_vol /= static_cast<float>(num_counted);
        cur_vol = sqrt(cur_vol);
        smoothed_vol *= 0.93;
        smoothed_vol += 0.07 * cur_vol;
    }
    
    void drawAudioInput(){
        ofPushMatrix();
        ofPushStyle();
        ofTranslate(20, 180);
        ofNoFill();
        ofDrawRectangle(0, 0, 240, 50);
        ofPopStyle();
        ofFill();
        ofDrawRectangle(0,0 , 240 * scaled_vol, 50);
        ofPopMatrix();
    }
    
    
private:
    ofSoundStream sound;
    vector<float> left;
    vector<float> right;
    float smoothed_vol;
    float scaled_vol;
    
};


//========================================================================
int main( ){
    ofSetupOpenGL(320,280,OF_WINDOW);            // <-------- setup the GL context
    
    // this kicks off the running of my app
    // can be OF_WINDOW or OF_FULLSCREEN
    // pass in width and height too:
    ofRunApp(new ofApp());
    
}

