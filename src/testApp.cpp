#include "testApp.h"

//PV

void testApp::setup(){
    //ofSetWindowPosition(-1500, 0);
    
    
    windowWidth = ofGetWidth();
    windowHeight = ofGetHeight();
    //OSC setting
    receiver.setup(PORT);
    cameraPos = ofPoint(0,0,0);
    
    //orbit
    //ofSetSmoothLighting(1);
    //light.enable();
    
    for (int i =0; i< SATELITE; i++) {
        orbit[i] = ofRandom(0, 2000);
        speed_x[i] = ofRandom(1, 5);
        speed_y[i] = ofRandom(1, 5);
        speed_z[i] = ofRandom(1, 5);
        //speed_x[i] = 1;
        //speed_y[i] = 1;
        //speed_z[i] = 1;
    }
    
    //particle
    
    for(int i =0; i < NUM_PARTICLES; i++)
    {
        particlePos[i].x = windowWidth*2 - ofRandomWidth()*4;
        particlePos[i].y = windowHeight*2 - ofRandomHeight()*4;
        
        particlePos[i].z = windowHeight*2 - ofRandomHeight()*4;
        //pos[i].z = 0;
        
        particleVel[i].x = ofRandomf();
        particleVel[i].y = ofRandomf();
        
        particleDrag[i] = ofRandom(0.95, 0.99);
    }
    vbo.setVertexData(particlePos, NUM_PARTICLES, GL_DYNAMIC_DRAW);
    
    ///////////////////////////////////////////////////////////////
    for(int i = 0; i < NUM; i++){
        posX[i] = ofRandom(windowWidth);
        posY[i] = ofRandom(windowHeight);
        speedX[i] = ofRandom(5)+ 2;
        speedY[i] = ofRandom(5)+ 2;
        color[i] = ofRandom(255);
        size[i] = ofRandom(255) + 20;
        rectPos[i] = windowHeight - ofRandom(windowHeight*2);
    }
   
    ofSetCircleResolution(48);
    
    ofDisableArbTex();
    
    //Load Shader
    shader.load("displace.vert", "displace.frag");
    fbo.allocate(width, height, GL_RGBA);
    
    ofFbo::Settings settings;
    settings.width = width;
    settings.height = height;
    settings.textureTarget = GL_TEXTURE_2D;
    settings.useDepth = true;
    settings.useStencil = true;
    settings.depthStencilAsTexture = true;
    fbo.allocate(settings);

    glPointSize(2);
    mainMesh.setMode(OF_PRIMITIVE_LINE_STRIP);
    //mainMesh.setMode(OF_PRIMITIVE_LINE_LOOP);
    //mainMesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    //mainMesh.setMode(OF_PRIMITIVE_POINTS);

    float sphereCompleteness = 1.0; // 1.0 for whole sphere, 0.5 for hemisphere
    unsigned int rings = 96;
    unsigned int sectors = 96;
    float const R = 1./(float)(rings-1) * sphereCompleteness;
    float const S = 1./(float)(sectors-1);
    int r, s;
    float radius = 500;
    for(r = 0; r < rings; r++) for(s = 0; s < sectors; s++) {
        float const y = sin( -M_PI_2 + M_PI * r * R );
        float const x = cos(2*M_PI * s * S) * sin( M_PI * r * R );
        float const z = sin(2*M_PI * s * S) * sin( M_PI * r * R );
        mainMesh.addTexCoord( ofVec2f(s*S, r*R) );
        mainMesh.addVertex(ofVec3f(x * radius, y * radius, z * radius));
        mainMesh.addNormal(ofVec3f(x,y,z));
    }
    for(r = 0; r < rings; r++) {
        for(s = 0; s < sectors; s++) {
            
            mainMesh.addIndex( r * sectors + s );
            mainMesh.addIndex( r * sectors + (s+1));
            mainMesh.addIndex( (r+1) * sectors + (s+1));
            mainMesh.addIndex( (r+1) * sectors + s);
        }
    }

    vbomesh = mainMesh;
}


void testApp::update() {
    /////////////////////   OSC    //////////////////////////////
    
    while (receiver.hasWaitingMessages()) {
        
        ofxOscMessage m;
        receiver.getNextMessage(&m);
        
        if(m.getAddress() == "/peakAmp"){
            
            peak = m.getArgAsFloat(0);
            
            
        }
        if(m.getAddress() == "/sphereTexMode"){
            sphereTexMode = m.getArgAsInt32(0);
            //cout <<"sphereTexMode : " << sphereTexMode <<endl;
            if(sphereTexMode == 1){
                vbomesh.setMode(OF_PRIMITIVE_LINE_LOOP);
            }
            if(sphereTexMode == 2){
                vbomesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
            }
            if(sphereTexMode == 3){
                vbomesh.setMode(OF_PRIMITIVE_POINTS);
            }
        }
        
        if(m.getAddress() == "/sphereMode"){
            
            sphereMode = m.getArgAsInt32(0);
            //cout << "sphereMode : " << sphereMode <<endl;
            
        }
        
        if(m.getAddress() == "/circleMode"){
            
            circleMode = m.getArgAsInt32(0);
            //cout << "circleMode : "<< circleMode <<endl;
            
        }
        
        if(m.getAddress() == "/rectMode"){
            
            rectMode = m.getArgAsInt32(0);
            //cout << "rectMode : " <<rectMode<<endl;
            
        }
        
        if(m.getAddress() == "/rectMode"){
            
            rectMode = m.getArgAsInt32(0);
            
        }
        
        if(m.getAddress() == "/cameraPos"){
            
            cameraPos.x = m.getArgAsFloat(0);
            cameraPos.y = m.getArgAsFloat(1);
            cameraPos.z = m.getArgAsFloat(2);
//            cout << "cameraPos.x : " <<cameraPos.x<<endl;
//            cout << "cameraPos.y : " <<cameraPos.z<<endl;
//            cout << "cameraPos.z : " <<cameraPos.z<<endl;
        }
        if(m.getAddress() == "/cameraPos_x"){
            
            cameraPos.x = m.getArgAsFloat(0);

            //            cout << "cameraPos.x : " <<cameraPos.x<<endl;

        }
        if(m.getAddress() == "/cameraPos_y"){
            
            cameraPos.y = m.getArgAsFloat(0);

            //            cout << "cameraPos.y : " <<cameraPos.z<<endl;
        }
        if(m.getAddress() == "/cameraPos_z"){

            cameraPos.z = m.getArgAsFloat(0);
            //            cout << "cameraPos.z : " <<cameraPos.z<<endl;
        }
        

        
    }//OSC End
    
//    cameraPos.x = cameraPos.x + (cameraPosNext.x - cameraPos.x) * transitionSpeed;
//    cameraPos.y = cameraPos.y + (cameraPosNext.y - cameraPos.x) * transitionSpeed;
//    cameraPos.z = cameraPos.z + (cameraPosNext.z - cameraPos.x) * transitionSpeed;
//    cout << "cameraPos.x : " <<cameraPos.x<<endl;
//    cout << "cameraPos.y : " <<cameraPos.z<<endl;
//    cout << "cameraPos.z : " <<cameraPos.z<<endl;
    
    for(int i = 0; i < NUM; i ++){
        posX[i] = posX[i] + speedX[i];
        posY[i] = posY[i] + speedY[i];
        
        if (posX[i] < 0) {
            speedX[i] = speedX[i] * -1;
            //posX = windowWidth;
        }
        
        if (posX[i] > windowWidth) {
            speedX[i] = speedX[i] * -1;
            //posX = 0;
        }
        
        if (posY[i] < 0) {
            speedY[i] = speedY[i] * -1;
            //posY = windowHeight;
            
        }
        if (posY[i] > windowHeight) {
            speedY[i] = speedY[i] * -1;
            //posY = 0;
        }
    }

    for (int i = 0; i < NUM; i++) {
        if (rectPos[i] < windowWidth) {
            rectPos[i] += 10;
        }else{
            rectPos[i] = 0;
        }
        
    }
    
    //particle
    //mX = ofMap(mouseX, 0, 1, 0, windowWidth);
    //mY = ofMap(mouseY, 0, 1, 0, windowHeight);
    
    for(int i=0; i <NUM_PARTICLES; i++){
        
        //ofPoint attractPt(mX, mY);
        particleFrc[i] = attractPt-particlePos[i]; // we get the attraction force/vector by looking at the mouse pos relative to our pos
        particleFrc[i].normalize(); //by normalizing we disregard how close the particle is to the attraction point
        
        particleVel[i] *= particleDrag[i]; //apply drag
        particleVel[i] += particleFrc[i] * 0.6; //apply force
        
        particlePos[i] += particleVel[i];

        
    }
    //orbit
    orbitSize = 500 + 10;
    //light.setPosition(mouseX, 0, -1000);
    for (int i =0; i< SATELITE; i++) {
        orbit[i] += 1 * peak ;
        float rad;
        rad = ofDegToRad(orbit[i]);
        y[i] = sin( -M_PI_2 + M_PI * rad )* speed_y[i] * orbitSize;
        x[i] = cos(2*M_PI * rad) * sin( M_PI * rad )* speed_x[i] * orbitSize;
        z[i] = sin(2*M_PI * rad ) * sin( M_PI * rad )* speed_z[i]* orbitSize;
        //particlePos[i] = ofPoint(x[i], y[i],z[i]);
    }
    
}
void testApp::draw(){
    ofBackground(0);
    
    
    //mCamMain.setPosition(ofVec3f(0, 0, -4000));
    
    //ofEnableDepthTest();
	// we don't care about alpha blending in this example, and by default alpha blending is on in openFrameworks > 0.8.0
	// so we de-activate it for now.
	//ofDisableAlphaBlending();
    ofDisableArbTex();
    
    //texture FBO
    float ballDepth;
    float rectDepth;
    if (sphereMode == 0) {
        ballDepth = 0;
        rectDepth = 0;
    }
    if (sphereMode == 1) {
        ballDepth = 1;
        rectDepth = 0;
    }
    if (sphereMode == 2) {
        ballDepth = 0;
        rectDepth = 1;
    }
    if (sphereMode == 3) {
        ballDepth = 1;
        rectDepth = 1;
    }
    
    fbo.begin();
        ofEnableAlphaBlending();
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        ofClear(0);
        ofBackground(16);
        
        ofFill();
    
    
        for (int i = 0; i < NUM; i++) {
            ofSetColor(color[i] * ballDepth, color[i + 1] * ballDepth, color[i + 2] * ballDepth, 128);
            ofCircle(posX[i], posY[i], size[i]/5 * peak * 4);
            
            ofSetColor(color[i] * rectDepth, color[i + 1] * rectDepth, color[i + 2] * rectDepth, 64);
            ofRect(0, rectPos[i], windowWidth*2, 5);

        }
        
    fbo.end();


    ofEnableArbTex();
    ofPushMatrix();
    mCamMain.disableMouseInput();
    ofTranslate(-windowWidth/2, -windowHeight/2, 0);
    mCamMain.setTarget(ofVec3f(0, 0, 200));
    mCamMain.setTarget(cameraPos); // <- camera position
    mCamMain.setDistance(3000);
    //ofNode node = mCamMain.getTarget();
    //float node = mCamMain.getDistance();
    //cout << "node pos = " << node.getPosition() << " node global pos = " << node.getGlobalPosition() << endl;
    //cout << "node distance = " << node << endl;
    
    mCamMain.begin();
    
    if (sphereMode == 1 || sphereMode == 2 ||sphereMode == 3) {
        ofPushMatrix();
    
            //cout << "test" << endl;
            shader.begin();
                shader.setUniformTexture("colormap", fbo.getTextureReference(), 0);
                //float scale = ofMap(mouseY, 0, windowHeight, 0.01, 3);
                float scale = ofMap(peak, 0, 1, 0.01, 3);
                shader.setUniform1f("peak",scale);
                ofScale(scale, scale, scale);
                vbomesh.draw();
            shader.end();
        
        ofPopMatrix();
        }
    
    ofPushMatrix();
    
        ofRotateX(90);
        //float size = ofMap(mouseY, 0, windowHeight, 0, 3);
    float size = ofMap(peak, 0, 1, 0, 3);
    if (circleMode == 1) {
        for (int i = 0; i < NUM; i++) {
            ofSetColor(255, 255, 255, 50);
            ofNoFill();
            ofCircle(0, 0, windowHeight * size/2 - (rectPos[i] * size), peak * 4500);
            
        }
    }
    
    if (rectMode == 1) {
        ofPushMatrix();
        ofRotate(360 - peak* 720, 360 - peak* 360 , 360 - peak* 360, deg);
        for (int i = 0; i < NUM; i++) {
            
            float yPos = ofRandom(windowHeight);
            ofSetColor(255, 255, 255, 25);
            ofFill();
            
            //ofPoint pos1 = ofVec3f(0 - windowWidth, (windowHeight/2- (yPos)) + 000, -windowHeight/2 + 375) * size;
            //ofPoint pos2 = ofVec3f(windowWidth, (windowHeight/2 - (yPos)) + 000, -windowHeight/2 + 375) * size;
            //ofLine(pos1, pos2);
            
            ofPoint pos = ofVec3f(-windowWidth/2  , (windowHeight/2- (yPos)) , (-windowHeight/2 + 500)) * size;
            ofRect(pos, windowWidth* size, ofRandom(1, 50));
            
        }
        ofPopMatrix();
    }
    
        //cout << mouseX << endl;
    
    //particle
    ofSetColor(255 * peak, 255 * peak, 255 * peak, 25);
    vbo.updateVertexData(particlePos, NUM_PARTICLES);
    glPointSize(4);
    vbo.draw(GL_POINTS, 0, NUM_PARTICLES);
    
    //ofPopMatrix();
    
    
    
    //orbit
    
    
    //ofTranslate(windowWidth/2, windowHeight/2,  -1000);
    //ofPushMatrix();
    
    ofRotate(deg);
    ofRotateX(90);
    for (int i =0; i< SATELITE; i++) {
        ofFill();
        ofSetColor(255, 255* peak, 50, 200);
        //ofDrawSphere(x[i], y[i], z[i], 20);
        ofCircle(x[i], y[i], z[i], 30 * peak);
        
    }
//    ofSetColor(255, 255, 255, 25);
//    vbo.updateVertexData(particlePos, SATELITE);
//    glPointSize(4);
//    vbo.draw(GL_POINTS, 0, SATELITE);
    
    ofPopMatrix();
    
    mCamMain.end();
    ofPopMatrix();
    ofDisableDepthTest();
    
    //display fbo texture
//    ofDisableAlphaBlending();
//    
//    fbo.draw(windowWidth/2, 0);
//    
//	ofSetColor(ofColor::white);
//
//	ofEnableAlphaBlending();
//    

}



//--------------------------------------------------------------
void testApp::keyPressed(int key){
//    if(key == '1'){
//        vbomesh.setMode(OF_PRIMITIVE_LINE_LOOP);
//    }
//    if(key == '2'){
//        vbomesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
//    }
//    if(key == '3'){
//        vbomesh.setMode(OF_PRIMITIVE_POINTS);
//    }
    

    if(key == 'f'){
        ofToggleFullscreen();
    }
    if(key == ' '){
        for(int i =0; i < NUM_PARTICLES; i++)
        {
            particlePos[i].x = windowWidth*8 - ofRandomWidth()*16;
            particlePos[i].y = windowHeight*8 - ofRandomHeight()*16;
            
            particlePos[i].z = windowHeight*8 - ofRandomHeight()*16;
            //pos[i].z = 0;
            
            particleVel[i].x = ofRandomf();
            particleVel[i].y = ofRandomf();
            
            particleDrag[i] = ofRandom(0.9, 0.99);
        }
//   attractPt = ofPoint(windowWidth - ofRandomWidth() * 2, windowHeight - ofRandomHeight() * 2, windowHeight - ofRandomHeight() * 2);
        attractPt = ofPoint(0, 0, 0);
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    windowWidth = w;
    windowHeight = h;
    ///////////////////////////////////////////////////////////////
    for(int i = 0; i < NUM; i++){
        posX[i] = ofRandom(windowWidth);
        posY[i] = ofRandom(windowHeight);
        speedX[i] = ofRandom(5)+ 2;
        speedY[i] = ofRandom(5)+ 2;
        color[i] = ofRandom(255);
        size[i] = ofRandom(255) + 20;
        rectPos[i] = windowHeight - ofRandom(windowHeight*2);
    }
    
    ofSetCircleResolution(60);
    
    ofDisableArbTex();
    
    //Load Shader
    //shader.load("displace.vert", "displace.frag");
    fbo.allocate(width, height, GL_RGBA);
    
    ofFbo::Settings settings;
    settings.width = width;
    settings.height = height;
    settings.textureTarget = GL_TEXTURE_2D;
    settings.useDepth = true;
    settings.useStencil = true;
    settings.depthStencilAsTexture = true;
    fbo.allocate(settings);

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
