#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    
    printGLInfo();

    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetVerticalSync(true);
	ofSetFrameRate(60);
    ofBackground(40);
    

    //shaderManager.load("tessellation");
    shaderManager.useLight(&spotLight);
    shaderManager.useLight(&directionalLight);
    shaderManager.useLight(&pointLight);
    shaderManager.useMaterial(&material);
    shaderManager.useCamera(&cam);
    
    shader = shaderManager.shader();
    
    shader->setGeometryInputType(GL_TRIANGLES);
    shader->setGeometryOutputType(GL_LINE_STRIP);
    shader->setGeometryOutputCount(4);
    
    
    shader->setupShaderFromFile(GL_VERTEX_SHADER, "tessellation.vert.glsl");
    shader->setupShaderFromFile(GL_FRAGMENT_SHADER, "tessellation.frag.glsl");
    shader->setupShaderFromFile(GL_GEOMETRY_SHADER_EXT, "tessellation.geom.glsl");
    shader->setupShaderFromFile(GL_TESS_CONTROL_SHADER, "tessellation.cont.glsl");
    shader->setupShaderFromFile(GL_TESS_EVALUATION_SHADER, "tessellation.eval.glsl");
    shader->linkProgram();
    
    // We work with 4 points per patch.
    glPatchParameteri(GL_PATCH_VERTICES, 4);
    
    setupVbos();
    setupLights();
    setupGui();
    
}



//--------------------------------------------------------------
void ofApp::update() {
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    updateLights();
    updateMaterial();
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofEnableDepthTest();
    cam.begin();
    
    ofDrawAxis(1000);
    
    shaderManager.begin();
    
    shaderManager.shader()->setUniform1f("u_tessLevelInner", tessLevelInner);
    shaderManager.shader()->setUniform1f("u_tessLevelOuter", tessLevelOuter);
    
    drawScene();
    shaderManager.end();
    
    //drawLights();

    cam.end();
    ofDisableDepthTest();

    
    gui.draw();
    ofDrawBitmapString(shaderManager.shaderName(), 20, ofGetHeight()-30);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case ' ':
            cout<<"*********************************"<<endl;
            cout<<"-- RELOAD:"<<endl;
            shader->unload();
            shader->setupShaderFromFile(GL_VERTEX_SHADER, "tessellation.vert.glsl");
            shader->setupShaderFromFile(GL_FRAGMENT_SHADER, "tessellation.frag.glsl");
            shader->setupShaderFromFile(GL_GEOMETRY_SHADER_EXT, "tessellation.geom.glsl");
            shader->setupShaderFromFile(GL_TESS_CONTROL_SHADER, "tessellation.cont.glsl");
            shader->setupShaderFromFile(GL_TESS_EVALUATION_SHADER, "tessellation.eval.glsl");
            shader->linkProgram();
            
            break;
        
        case '0':
            shaderManager.load("blank");
            break;
       
            break;
     

		default:
			break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}
//--------------------------------------------------------------

void ofApp::drawScene(){
    
    //SPHERE
    ofSetColor(255, 255, 255, 255);
    ofPushMatrix();
    ofTranslate(center.x, center.y, center.z-300);
    ofRotate(ofGetElapsedTimef() * .8 * RAD_TO_DEG, 0, 1, 0);
    
    //sphereVbo.drawElements(GL_TRIANGLE_STRIP,  sphereVbo.getNumIndices());
    
    sphereVbo.drawElements(GL_PATCHES, sphereVbo.getNumIndices());
    
    ofPopMatrix();
    
    //CYLINDER
    ofPushMatrix();
    ofTranslate(300, 300, cos(ofGetElapsedTimef()*1.4) * 300.f);
    ofRotate(ofGetElapsedTimef()*.6 * RAD_TO_DEG, 1, 0, 0);
    ofRotate(ofGetElapsedTimef()*.8 * RAD_TO_DEG, 0, 1, 0);
    cylinderVbo.drawElements(GL_PATCHES,  cylinderVbo.getNumIndices());
    ofPopMatrix();
    
    //BOX
    ofPushMatrix();
    ofTranslate(center.x, center.y, -1200);
    ofRotate(ofGetElapsedTimef() * .2 * RAD_TO_DEG, 0, 1, 0);
    boxVbo.drawElements(GL_PATCHES,  boxVbo.getNumIndices());
    ofPopMatrix();
    
    
}
//--------------------------------------------------------------
void ofApp::setupVbos(){
    
    ofIcoSpherePrimitive sphere;
    //ofSpherePrimitive sphere;
    ofCylinderPrimitive cylinder;
    ofBoxPrimitive box;
    ofPlanePrimitive plane;
    
    
    radius		= 180.f;
    center.set(0, 0, 0);
    
    sphere.setRadius(radius);
    sphere.setResolution(1);
    ofVboMesh sphereMesh = sphere.getMesh();
    sphereVbo.setMesh(sphereMesh, GL_DYNAMIC_DRAW);
    
    box.set(850);
    box.setResolution(1);
    ofVboMesh boxMesh = box.getMesh();
    boxVbo = boxMesh.getVbo();
    
    cylinder.set(70, 150);
    ofVboMesh cylinderMesh = cylinder.getMesh();
    cylinderVbo = cylinderMesh.getVbo();

}
//--------------------------------------------------------------
void ofApp::setupLights(){

    pointLight.setDiffuseColor( ofColor(0.f, 255.f, 0.f));
    pointLight.setSpecularColor( ofColor(255.f, 255.f, 0.f));
    pointLight.setPointLight();
    
    spotLight.setDiffuseColor( ofColor(255.f, 0.f, 0.f));
    spotLight.setSpecularColor( ofColor(255.f, 255.f, 255.f));
    spotLight.setSpotlight();
    spotLight.setSpotlightCutOff( 50 );
    spotLight.setSpotConcentration( 45 );
    
    directionalLight.setDiffuseColor(ofColor(0.f, 0.f, 255.f));
    directionalLight.setSpecularColor(ofColor(255.f, 255.f, 255.f));
    directionalLight.setDirectional();
    directionalLight.setOrientation( ofVec3f(0, 90, 0) );

}
//--------------------------------------------------------------
void ofApp::updateLights(){
    
    pointLight.setPosition(cos(ofGetElapsedTimef()*.6f) * radius * 2 + center.x,
                           sin(ofGetElapsedTimef()*.8f) * radius * 2 + center.y,
                           -cos(ofGetElapsedTimef()*.8f) * radius * 2 + center.z);
    
    spotLight.setOrientation( ofVec3f( 0, cos(ofGetElapsedTimef()) * RAD_TO_DEG, 0) );
    spotLight.setPosition( mouseX, mouseY, 200);
    
    spotLight.setSpotConcentration(spotConcentration);
    spotLight.setSpotlightCutOff(spotCutOff);
    
}
//--------------------------------------------------------------
void ofApp::updateMaterial(){
    
    material.setColors(diffuse, ambient, specular, emissive);
    material.setShininess(shininess);
    
}

//--------------------------------------------------------------
void ofApp::drawLights(){
    
    ofSetColor( pointLight.getDiffuseColor() );
    if(bPointLight) pointLight.draw();
    
    ofSetColor(255, 255, 255);
    if(bDirLight) directionalLight.draw();
    
    ofSetColor( spotLight.getDiffuseColor() );
    if(bSpotLight) spotLight.draw();
    
}
//--------------------------------------------------------------

void ofApp::printGLInfo(){
    
    cout<<"IsGLProgrammableRederer: "<< ofIsGLProgrammableRenderer() <<endl;
    cout<<"GL-Version-Major: "<< ofGetGLRenderer()->getGLVersionMajor() <<endl;
    cout<<"GL-Version-Minor: "<< ofGetGLRenderer()->getGLVersionMinor() <<endl;
    cout<<"GLSL Version: "<<ofGLSLVersionFromGL(ofGetGLRenderer()->getGLVersionMajor(), ofGetGLRenderer()->getGLVersionMinor())<<endl;

}
//--------------------------------------------------------------
void ofApp::setupGui(){
    
    bPointLight.addListener(this, &ofApp::pointLightChanged);
    bSpotLight.addListener(this, &ofApp::spotLightChanged);
    bDirLight.addListener(this, &ofApp::dirLightChanged);
    
    
    
    gui.setup();
    
    lightParameters.setName("Lights");
    lightParameters.add(bPointLight.set("PointLight",true));
    lightParameters.add(bDirLight.set("DirectionalLight", true));
    lightParameters.add(bSpotLight.set("SpotLight", true));
    lightParameters.add(spotCutOff.set("Spot CuttOff", 50, 0, 90));
    lightParameters.add(spotConcentration.set("Spot Concentr.", 45, 0, 128));
    
    
    materialParameters.setName("Material");
    materialParameters.add(shininess.set("shinines", 100, 0, 128));
    
    materialParameters.add(diffuse.set("diffuse", ofFloatColor(0.8, 1.0),
                                       ofFloatColor(0.0), ofFloatColor(1.0)) );
    materialParameters.add(ambient.set("ambient", ofFloatColor(0.2, 1.0),
                                       ofFloatColor(0.0), ofFloatColor(1.0)) );
    materialParameters.add(specular.set("specular", ofFloatColor(1.0, 1.0),
                                        ofFloatColor(0.0), ofFloatColor(0.5)));
    materialParameters.add(emissive.set("emmisive", ofFloatColor(0.0, 1.0),
                                        ofFloatColor(0.0), ofFloatColor(1.0)));
    
    gui.add(tessLevelInner.set("INNER tess lev.", 1.0, 1.0, 5.0));
    gui.add(tessLevelOuter.set("OUTER tess lev, ", 1.0, 1.0, 5.0));
    
    gui.add(lightParameters);
    gui.add(materialParameters);
    gui.add(bUseTexture.set("Use Texture",true));
    
   
}
//--------------------------------------------------------------
void ofApp::pointLightChanged(bool & bPointLight){
    shaderManager.toggleLight(&pointLight, bPointLight);
}
//--------------------------------------------------------------
void ofApp::spotLightChanged(bool & bSpotLight){
    shaderManager.toggleLight(&spotLight, bSpotLight);
}
//--------------------------------------------------------------
void ofApp::dirLightChanged(bool & bDirLight){
   shaderManager.toggleLight(&directionalLight, bDirLight);
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