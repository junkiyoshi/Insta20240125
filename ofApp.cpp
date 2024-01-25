#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openframeworks");

	ofBackground(39);
	ofEnableDepthTest();
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);

	ofSetLineWidth(1.5);

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
	this->noise_param = ofRandom(1000);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	if (ofGetFrameNum() % 50 < 25) {

		this->noise_param += ofMap(ofGetFrameNum() % 50, 0, 25, 3, 0.01);
	}
	else {

		this->noise_param += 0.01;
	}

	this->face.clear();
	this->frame.clear();

	frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);

	auto base_noise_seed = glm::vec3(ofRandom(1000), ofRandom(1000), ofRandom(1000));
	this->camera_location = glm::vec3(
		ofMap(ofNoise(base_noise_seed.x, this->noise_param * 0.015), 0, 1, -400, 400),
		ofMap(ofNoise(base_noise_seed.y, this->noise_param * 0.015), 0, 1, -400, 400),
		ofMap(ofNoise(base_noise_seed.z, this->noise_param * 0.015), 0, 1, -400, 400));

	this->target_location = glm::vec3(
		ofMap(ofNoise(base_noise_seed.x, (this->noise_param + 30) * 0.015), 0, 1, -400, 400),
		ofMap(ofNoise(base_noise_seed.y, (this->noise_param + 30) * 0.015), 0, 1, -400, 400),
		ofMap(ofNoise(base_noise_seed.z, (this->noise_param + 30) * 0.015), 0, 1, -400, 400));

	for (int k = 0; k < 7; k++) {

		auto noise_seed = glm::vec3(ofRandom(1000), ofRandom(1000), ofRandom(1000));
		auto start_index = this->face.getNumVertices();

		ofColor color;
		for (int i = 0; i < 50; i++) {

			auto param = glm::vec3(25 - abs(i - 25), 0, 0);
			if (param.x > 15) { param.x = 15; }

			int index = this->face.getNumVertices();

			auto base_location = glm::vec3(
				ofMap(ofNoise(base_noise_seed.x, (i + this->noise_param) * 0.015), 0, 1, -400, 400),
				ofMap(ofNoise(base_noise_seed.y, (i + this->noise_param) * 0.015), 0, 1, -400, 400),
				ofMap(ofNoise(base_noise_seed.z, (i + this->noise_param) * 0.015), 0, 1, -400, 400));

			auto location = glm::vec3(
				ofMap(ofNoise(noise_seed.x, (i * 0.02 + this->noise_param * 0.05)), 0, 1, -200, 200),
				ofMap(ofNoise(noise_seed.y, (i * 0.02 + this->noise_param * 0.05)), 0, 1, -200, 200),
				ofMap(ofNoise(noise_seed.z, (i * 0.02 + this->noise_param * 0.05)), 0, 1, -200, 200));

			location += base_location;

			this->face.addVertex(location + param);
			this->face.addVertex(location - param);

			color.setHsb(ofMap(k, 0, 7, 0, 255), 255, 255);

			this->face.addColor(ofColor(color, 128));
			this->face.addColor(ofColor(color, 128));

			this->frame.addVertex(location + param);
			this->frame.addVertex(location - param);

			this->frame.addColor(color);
			this->frame.addColor(color);

			if (i > 0) {

				this->face.addIndex(index + 0); this->face.addIndex(index - 1); this->face.addIndex(index - 2);
				this->face.addIndex(index + 0); this->face.addIndex(index + 1); this->face.addIndex(index - 1);

				this->frame.addIndex(index + 0); this->frame.addIndex(index - 2);
				this->frame.addIndex(index + 1); this->frame.addIndex(index - 1);
			}
		}

		this->frame.addIndex(start_index + 0); this->frame.addIndex(start_index + 1);
		this->frame.addIndex(frame.getNumVertices() - 1); this->frame.addIndex(frame.getNumVertices() - 2);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	auto camera_location = this->camera_location + glm::vec3(0, 0, 500);

	this->cam.setPosition(camera_location);
	this->cam.setTarget(target_location);
	this->cam.lookAt(target_location);

	this->cam.begin();

	this->face.drawFaces();

	ofSetColor(239);
	this->frame.drawWireframe();

	this->cam.end();

	/*
	int start = 100;
	if (ofGetFrameNum() > start) {

		ostringstream os;
		os << setw(4) << setfill('0') << ofGetFrameNum() - start;
		ofImage image;
		image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		image.saveImage("image/cap/img_" + os.str() + ".jpg");
		if (ofGetFrameNum() - start >= 25 * 20) {

			std::exit(1);
		}
	}
	*/
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}