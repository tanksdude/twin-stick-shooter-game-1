//TODO: #includes

int main(int argc, char** argv) {
	//TODO: very obviously not done
	std::string name = "Twin-Stick Shooter Test Game v0.0.9"; //v0.1.0 once everything compiles
	Renderer::PreInitialize(&argc, argv, name, 60, 60, 600, 600);

	// Set callback for drawing the scene
	glutDisplayFunc(GameSceneManager::DrawScenes);

	// Set callback for resizing the window
	glutReshapeFunc(Renderer::windowResizeFunc);

	//mouse clicking
	glutMouseFunc(DeveloperManager::mouseClickFunc);

	// Set callback to handle mouse dragging
	glutMotionFunc(DeveloperManager::mouseDragFunc);

	//passive mouse dragging
	//TODO

	// Set callback to handle keyboard events
	glutKeyboardFunc(KeypressManager::setNormalKey);

	//callback for keyboard up events
	glutKeyboardUpFunc(KeypressManager::unsetNormalKey);

	//special keyboard down
	glutSpecialFunc(KeypressManager::setSpecialKey);

	//special keyboard up
	glutSpecialUpFunc(KeypressManager::unsetSpecialKey);

	//mousewheel
	glutMouseWheelFunc(DeveloperManager::mouseWheelFunc);

	// Set callback for the idle function
	//glutIdleFunc(draw);

	//initialize managers and stuff:
	GameManager::Initialize();
	KeypressManager::Initialize();
	BackgroundRect::initializeGPU()

	//game mode:
	GameSceneManager::pushScene(new GameMainLoop());

	//main game code initialization stuff:
	//TODO

	//std::cout << "OpenGL renderer: " << glGetString(GL_RENDERER) << std::endl;
	//std::cout << "OpenGL vendor: " << glGetString(GL_VENDOR) << std::endl;
	//std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl << std::endl;

	//framelimiter
	GameSceneManager::TickScenes(100);

	// Start the main loop
	glutMainLoop();
}
