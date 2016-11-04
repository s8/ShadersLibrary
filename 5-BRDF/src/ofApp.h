#pragma once

#include "ofMain.h"

#include "LightShaderManager.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void keyPressed  (int key);
    
    void setupLights();
    void setupGui();
    
    void updateLights();
    void updateMaterial();
    
    void drawScene();
    void drawLights();
    
    void printGLInfo();
    
    ofLight pointLight;
	ofLight spotLight;
	ofLight directionalLight;
	
	ofMaterial material;
    ofImage logoImage;
    
    ofEasyCam cam;
		
    ofSpherePrimitive sphere;
    ofCylinderPrimitive cylinder;
    ofBoxPrimitive box;
    
    float radius;
    ofVec3f center;
    
    LightShaderManager shaderManager;
    
    ofxPanel gui;
    
    ofParameterGroup lightParameters;
    ofParameter<bool> bPointLight,  bSpotLight, bDirLight;
    ofParameter<int> spotCutOff; //0-90 degrees
    ofParameter<int> spotConcentration;// 0 - 128
    
    ofParameterGroup materialParameters;
    ofParameter<ofFloatColor> diffuse;
    ofParameter<ofFloatColor> ambient;
    ofParameter<ofFloatColor> specular;
    ofParameter<ofFloatColor> emissive;
    ofParameter<float> shininess;//0-128

    ofParameter<bool> bUseTexture;
    
    ofParameterGroup uniforms;
    ofParameter<ofVec2f> P;
    ofParameter<ofVec2f> A;
    ofParameter<ofVec3f> Scale;
    
    void pointLightChanged(bool & bPointLight);
    void spotLightChanged(bool & bSpotLight);
    void dirLightChanged(bool & bDirLight);
    void textureToggled(bool & bUseTexture);
    
};
