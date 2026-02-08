#include "Game.h"
#include <iostream>
#include <cmath>
#include <sstream>
#include <filesystem>
namespace fs = std::filesystem;

const float SCALE = 10.0f;

Game::Game() :
    state(GameState::MENU_MAIN),
    currentLevelIndex(0),
    scor(0),
    isDragging(false),
    titleText(font),
    playButtonText(font),
    exitButtonText(font),
    backButtonText(font),
    backToTitleText(font),
    levelSelectTitle(font),
    levelCompleteText(font),
    playSprite(playTexture),
    exitSprite(exitTexture),
    backSprite(backTexture),
    backgroundSprite(backgroundTexture),
    grassSprite(grassTexture),
    holeSound(holeSoundBuffer),
    levelCompleteSound(levelCompleteBuffer)
{
    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "EROARE: Nu am putut incarca arial.ttf\n";
    }
    // Pixelated Text Style
    const_cast<sf::Texture&>(font.getTexture(30)).setSmooth(false);
    const_cast<sf::Texture&>(font.getTexture(40)).setSmooth(false);
    const_cast<sf::Texture&>(font.getTexture(50)).setSmooth(false);
    const_cast<sf::Texture&>(font.getTexture(60)).setSmooth(false);
    initLevels();
    initUI();
}

void Game::initUI() {
    // Main Menu
    titleText.setString("2D GOLF");
    titleText.setCharacterSize(80); // Bigger
    titleText.setFillColor(sf::Color(100, 255, 100)); // Nice Green
    titleText.setOutlineColor(sf::Color::White);
    titleText.setOutlineThickness(3.0f);
    titleText.setStyle(sf::Text::Style::Bold);
    // Center Title
    sf::FloatRect tr = titleText.getLocalBounds();
    titleText.setOrigin({tr.size.x/2.0f, tr.size.y/2.0f}); // SFML 3: size.x, size.y
    titleText.setPosition({400.f, 100.f}); // Centered X, higher Y

    playButtonText.setString("PLAY");
    playButtonText.setCharacterSize(40);
    playButtonText.setFillColor(sf::Color::White);
    playButtonText.setOutlineColor(sf::Color::Black);
    playButtonText.setOutlineThickness(2.0f);
    // Centered logic will apply in render or here if we knew texture size.
    // Let's set generic positions for now, refined in render.
    playButtonText.setPosition({350.f, 250.f});

    exitButtonText.setString("EXIT");
    exitButtonText.setCharacterSize(40);
    exitButtonText.setFillColor(sf::Color::White);
    exitButtonText.setOutlineColor(sf::Color::Black);
    exitButtonText.setOutlineThickness(2.0f);
    exitButtonText.setPosition({350.f, 350.f});

    // Level Select
    levelSelectTitle.setString("SELECT LEVEL");
    levelSelectTitle.setCharacterSize(40);
    levelSelectTitle.setFillColor(sf::Color::White);
    levelSelectTitle.setPosition({250.f, 20.f});

    // In-Game UI
    backButtonText.setString("MENU");
    backButtonText.setCharacterSize(20);
    backButtonText.setFillColor(sf::Color::Yellow);
    backButtonText.setPosition({10.f, 10.f});

    // [NEW] Level Select Back Button
    backToTitleText.setString("BACK");
    backToTitleText.setCharacterSize(24);
    backToTitleText.setFillColor(sf::Color::Yellow);
    backToTitleText.setPosition({10.f, 10.f}); // Top Left

    // Music Loading Removed
    // levelMusic loading removed

    // Load SFX (Try mp3, fallback ogg if needed but user said mp3 works)
    std::string soundPaths[] = { "gaura.mp3", "../gaura.mp3", "../cmake-build-debug/gaura.mp3" };
    bool holeLoaded = false;
    for(const auto& p : soundPaths) {
        if(fs::exists(p)) {
            if(holeSoundBuffer.loadFromFile(p)) {
                 holeLoaded = true;
                 break;
            }
        }
    }
    if(!holeLoaded) std::cerr << "Warning: gaura.mp3 not found.\n";
    holeSound.setBuffer(holeSoundBuffer);

    // [NEW] Level Complete Sound
    std::string levelPaths[] = { "level_complete.mp3", "../level_complete.mp3", "../cmake-build-debug/level_complete.mp3" };
    bool levelLoaded = false;
     for(const auto& p : levelPaths) {
        if(fs::exists(p)) {
            if(levelCompleteBuffer.loadFromFile(p)) {
                 levelLoaded = true;
                 break;
            }
        }
    }
    if(!levelLoaded) std::cerr << "Warning: level_complete.mp3 not found.\n";
    levelCompleteSound.setBuffer(levelCompleteBuffer);

    // Helper lambda for robust loading
    // CWD debug removed


    auto loadTex = [&](sf::Texture& tex, const std::string& filename) {
        // [NEW] Check existence first to avoid SFML error logging
        std::string paths[] = {
            filename, // 1. Relative (CWD)
            "../" + filename, // 2. Parent directory
            "../cmake-build-debug/" + filename // 3. Build directory (fallback)
        };

        for (const auto& path : paths) {
            if (fs::exists(path)) {
                if (tex.loadFromFile(path)) {
                    std::cout << "Loaded texture: " << path << "\n";
                    return true;
                }
            }
        }

        // Only print error if ALL attempts fail (and file likely doesn't exist)
        std::cerr << "Failed to load texture: " << filename << " (checked multiple paths)\n";
        return false;
    };

    // Load Textures (Assets)
    loadTex(playTexture, "play.png");
    loadTex(exitTexture, "exit.png");
    loadTex(backTexture, "menu.png");
    loadTex(backgroundTexture, "meniu2.png");
    loadTex(levelBtnTexture, "menu_button.png");

    // Default positioning for sprites if textures loaded
    if(playTexture.getSize().x > 0) {
        playSprite.setTexture(playTexture, true);
        sf::FloatRect b = playSprite.getLocalBounds();
        playSprite.setOrigin({b.size.x/2.f, b.size.y/2.f});
        playSprite.setPosition({200.f, 480.f}); // Left
        // Scale to ~200px width
        float scale = 200.0f / b.size.x;
        playSprite.setScale({scale, scale});
    }
    if(exitTexture.getSize().x > 0) {
        exitSprite.setTexture(exitTexture, true);
        sf::FloatRect b = exitSprite.getLocalBounds();
        exitSprite.setOrigin({b.size.x/2.f, b.size.y/2.f});
        exitSprite.setPosition({600.f, 480.f}); // Right
        // Scale to ~200px width
        float scale = 200.0f / b.size.x;
        exitSprite.setScale({scale, scale});
    }
    if(backTexture.getSize().x > 0) {
        backSprite.setTexture(backTexture, true); // [FIX] Reset Rect
        backSprite.setPosition({10.f, 10.f});
        sf::Vector2u sz = backTexture.getSize();
        if (sz.x > 0) {
             float sc = 40.0f / (float)sz.x;
             backSprite.setScale({sc, sc});
        }
    }
    // Background scaling
    if (backgroundTexture.getSize().x > 0) {
        backgroundSprite.setTexture(backgroundTexture, true); // [FIX] Reset Rect
        sf::Vector2u sz = backgroundTexture.getSize();
        float scaleX = 800.0f / (float)sz.x;
        float scaleY = 600.0f / (float)sz.y;
        backgroundSprite.setScale({scaleX, scaleY});
    }

    // Debug Text Removed

    if (loadTex(wallTexture, "wall.png")) {
        wallTexture.setRepeated(true);
    }
    if (loadTex(waterTexture, "water.png")) {
        waterTexture.setRepeated(true);
    }
    if (loadTex(sandTexture, "sand.png")) {
        sandTexture.setRepeated(true);
    }
    loadTex(holeTexture, "hole.png");
    if (loadTex(grassTexture, "grass.png")) {
        grassTexture.setRepeated(true);
        grassSprite.setTextureRect(sf::IntRect({0, 0}, {800, 600}));
    }
    loadTex(ballTexture, "ball.png");

    // Tee Removed

    // Assign textures to pre-loaded levels
    for (auto& lvl : nivele) {
        for (auto& obs : lvl.getObstacole()) {
            if (obs->tip() == "Perete") obs->setTexture(&wallTexture);
            else if (obs->tip() == "Nisip") obs->setTexture(&sandTexture);
            else if (obs->tip() == "Apa") obs->setTexture(&waterTexture);
        }
        lvl.getHole().setTexture(&holeTexture);
        lvl.getMinge().setTexture(&ballTexture);
    }
}

void Game::initLevels() {
    nivele.clear();
    // Level 1: Basics
    {
        Ball b(Vector2D(5, 30));
        Hole h(Vector2D(70, 30), 1.2f);
        std::vector<std::shared_ptr<Obstacle>> obs;
        obs.push_back(std::make_shared<Wall>(0, 80, 0, 1));
        obs.push_back(std::make_shared<Wall>(0, 80, 59, 60));
        obs.push_back(std::make_shared<Wall>(0, 1, 0, 60));
        obs.push_back(std::make_shared<Wall>(79, 80, 0, 60));
        nivele.emplace_back(b, h, obs);
    }
    // Level 2: Simple Obstacle
    {
        Ball b(Vector2D(5, 30));
        Hole h(Vector2D(70, 30), 1.2f);
        std::vector<std::shared_ptr<Obstacle>> obs;
        obs.push_back(std::make_shared<Wall>(0, 80, 0, 1)); obs.push_back(std::make_shared<Wall>(0, 80, 59, 60));
        obs.push_back(std::make_shared<Wall>(0, 1, 0, 60)); obs.push_back(std::make_shared<Wall>(79, 80, 0, 60));
        obs.push_back(std::make_shared<Wall>(35, 37, 10, 50));
        nivele.emplace_back(b, h, obs);
    }
    // Level 3: Sand Trap
    {
        Ball b(Vector2D(5, 50));
        Hole h(Vector2D(70, 10), 1.2f);
        std::vector<std::shared_ptr<Obstacle>> obs;
        obs.push_back(std::make_shared<Wall>(0, 80, 0, 1)); obs.push_back(std::make_shared<Wall>(0, 80, 59, 60));
        obs.push_back(std::make_shared<Wall>(0, 1, 0, 60)); obs.push_back(std::make_shared<Wall>(79, 80, 0, 60));
        obs.push_back(std::make_shared<Sand>(20, 60, 20, 40));
        nivele.emplace_back(b, h, obs);
    }
    // Level 4: Water Hazard
    {
        Ball b(Vector2D(5, 30));
        Hole h(Vector2D(70, 30), 1.2f);
        std::vector<std::shared_ptr<Obstacle>> obs;
        obs.push_back(std::make_shared<Wall>(0, 80, 0, 1)); obs.push_back(std::make_shared<Wall>(0, 80, 59, 60));
        obs.push_back(std::make_shared<Wall>(0, 1, 0, 60)); obs.push_back(std::make_shared<Wall>(79, 80, 0, 60));
        obs.push_back(std::make_shared<Water>(30, 40, 0, 60));
        nivele.emplace_back(b, h, obs);
    }
    // Level 5: The Maze
    {
        Ball b(Vector2D(5, 5));
        Hole h(Vector2D(75, 55), 1.2f);
        std::vector<std::shared_ptr<Obstacle>> obs;
        obs.push_back(std::make_shared<Wall>(0, 80, 0, 1)); obs.push_back(std::make_shared<Wall>(0, 80, 59, 60));
        obs.push_back(std::make_shared<Wall>(0, 1, 0, 60)); obs.push_back(std::make_shared<Wall>(79, 80, 0, 60));
        obs.push_back(std::make_shared<Wall>(15, 65, 15, 17));
        obs.push_back(std::make_shared<Wall>(15, 65, 43, 45));
        nivele.emplace_back(b, h, obs);
    }
    // Level 6: Black Hole Fun
    {
        Ball b(Vector2D(10, 30));
        Hole h(Vector2D(70, 30), 1.2f);
        std::vector<std::shared_ptr<Obstacle>> obs;
        obs.push_back(std::make_shared<Wall>(0, 80, 0, 1)); obs.push_back(std::make_shared<Wall>(0, 80, 59, 60));
        obs.push_back(std::make_shared<Wall>(0, 1, 0, 60)); obs.push_back(std::make_shared<Wall>(79, 80, 0, 60));
        auto gh = std::make_shared<BlackHole>(Vector2D(40, 30), 10.0f, 2.0f, 200.0f);
        auto wh = std::make_shared<WhiteHole>(Vector2D(60, 10), 5.0f, 25.0f);
        gh->seteazaDestinatie(wh);
        wh->seteazaSursa(gh);
        obs.push_back(gh);
        obs.push_back(wh);
        nivele.emplace_back(b, h, obs);
    }
     // Level 7: Pinball
    {
        Ball b(Vector2D(40, 50));
        Hole h(Vector2D(40, 10), 1.2f);
        std::vector<std::shared_ptr<Obstacle>> obs;
        obs.push_back(std::make_shared<Wall>(0, 80, 0, 1)); obs.push_back(std::make_shared<Wall>(0, 80, 59, 60));
        obs.push_back(std::make_shared<Wall>(0, 1, 0, 60)); obs.push_back(std::make_shared<Wall>(79, 80, 0, 60));
        obs.push_back(std::make_shared<WhiteHole>(Vector2D(20, 30), 3.0f, 200.0f));
        obs.push_back(std::make_shared<WhiteHole>(Vector2D(60, 30), 3.0f, 200.0f));
        obs.push_back(std::make_shared<WhiteHole>(Vector2D(40, 30), 2.0f, 200.0f));
        nivele.emplace_back(b, h, obs);
    }
    // Level 8: Narrow Path
    {
        Ball b(Vector2D(5, 30));
        Hole h(Vector2D(75, 30), 1.2f);
        std::vector<std::shared_ptr<Obstacle>> obs;
        obs.push_back(std::make_shared<Wall>(0, 80, 0, 1)); obs.push_back(std::make_shared<Wall>(0, 80, 59, 60));
        obs.push_back(std::make_shared<Wall>(0, 1, 0, 60)); obs.push_back(std::make_shared<Wall>(79, 80, 0, 60));
        obs.push_back(std::make_shared<Water>(0, 80, 0, 25));
        obs.push_back(std::make_shared<Water>(0, 80, 35, 60));
        nivele.emplace_back(b, h, obs);
    }
     // Level 9: The Fortress
    {
        Ball b(Vector2D(5, 5));
        Hole h(Vector2D(40, 30), 1.2f);
        std::vector<std::shared_ptr<Obstacle>> obs;
        obs.push_back(std::make_shared<Wall>(0, 80, 0, 1)); obs.push_back(std::make_shared<Wall>(0, 80, 59, 60));
        obs.push_back(std::make_shared<Wall>(0, 1, 0, 60)); obs.push_back(std::make_shared<Wall>(79, 80, 0, 60));
        obs.push_back(std::make_shared<Wall>(30, 50, 20, 22));
        obs.push_back(std::make_shared<Wall>(30, 50, 38, 40));
        obs.push_back(std::make_shared<Wall>(30, 32, 20, 40));
        obs.push_back(std::make_shared<Sand>(50, 60, 20, 40));
        nivele.emplace_back(b, h, obs);
    }
    // Level 10: Impossible?
    {
        Ball b(Vector2D(4, 4));
        Hole h(Vector2D(76, 56), 1.2f);
        std::vector<std::shared_ptr<Obstacle>> obs;
        obs.push_back(std::make_shared<Wall>(0, 80, 0, 1)); obs.push_back(std::make_shared<Wall>(0, 80, 59, 60));
        obs.push_back(std::make_shared<Wall>(0, 1, 0, 60)); obs.push_back(std::make_shared<Wall>(79, 80, 0, 60));
        obs.push_back(std::make_shared<Water>(10, 20, 0, 60));
        obs.push_back(std::make_shared<Sand>(30, 40, 0, 60));
        obs.push_back(std::make_shared<Wall>(50, 52, 10, 50));
        auto gh = std::make_shared<BlackHole>(Vector2D(60, 30), 8.0f, 1.0f, 250.0f);
        auto wh = std::make_shared<WhiteHole>(Vector2D(15, 30), 3.0f);
        gh->seteazaDestinatie(wh); wh->seteazaSursa(gh);
        obs.push_back(gh); obs.push_back(wh);
        nivele.emplace_back(b, h, obs);
    }
    // Level 11: Physics Test
    {
        Ball b(Vector2D(10, 30));
        Hole h(Vector2D(70, 30), 1.2f);
        std::vector<std::shared_ptr<Obstacle>> obs;
        obs.push_back(std::make_shared<Wall>(0, 80, 0, 1)); obs.push_back(std::make_shared<Wall>(0, 80, 59, 60));
        obs.push_back(std::make_shared<Wall>(0, 1, 0, 60)); obs.push_back(std::make_shared<Wall>(79, 80, 0, 60));
        obs.push_back(std::make_shared<Wall>(30, 31, 10, 50));
        obs.push_back(std::make_shared<Wall>(34, 35, 10, 50));
        nivele.emplace_back(b, h, obs);
    }
}

void Game::generateThumbnails() {
    sf::RenderTexture rt;
    if (!rt.resize({800, 600})) {
        std::cerr << "Failed to create render texture for thumbnails\n";
        return;
    }

    levelThumbnails.clear();
    levelThumbnailSprites.clear();
    levelButtons.clear();
    levelButtonLabels.clear();

    levelThumbnails.reserve(nivele.size());

    int cols = 5;
    int w = 140;
    int h = 100;
    int padX = 15;
    int padY = 60;
    int startX = 20;
    int startY = 80;

    for (size_t i = 0; i < nivele.size(); ++i) {
        rt.clear(sf::Color(30, 100, 30));
        nivele[i].render(rt);
        rt.display();

        levelThumbnails.push_back(rt.getTexture());
        sf::Sprite sprite(levelThumbnails.back());
        float scaleX = (float)w / 800.0f;
        float scaleY = (float)h / 600.0f;
        sprite.setScale({scaleX, scaleY});

        int r = i / cols;
        int c = i % cols;
        float posX = startX + c * (w + padX);
        float posY = startY + r * (h + padY); // Fixed from previous logical issue?
                                              // Previous was: startY + r * (h + padY); -> Looks OK.

        sprite.setPosition({posX, posY});
        levelThumbnailSprites.push_back(sprite);
        // Border/Button
        sf::RectangleShape btn(sf::Vector2f(w, h));
        btn.setPosition({posX, posY});

        // Custom Level Texture?
        if (levelBtnTexture.getSize().x > 0) {
            btn.setTexture(&levelBtnTexture);
            btn.setFillColor(sf::Color::White);
        } else {
            btn.setFillColor(sf::Color::Transparent);
            btn.setOutlineThickness(2);
            btn.setOutlineColor(sf::Color::White);
        }

        levelButtons.push_back(btn);

        sf::Text label(font);
        std::stringstream ss; ss << "Level " << i + 1;
        label.setString(ss.str());
        label.setCharacterSize(18);
        label.setPosition({posX + 10.f, posY + 110.f});
        label.setFillColor(sf::Color::White);
        levelButtonLabels.push_back(label);
    }
}

void Game::start() {
    window.create(sf::VideoMode({800, 600}), "2D Golf v2.0");
    window.setFramerateLimit(60);
    sf::View view = window.getDefaultView();
    generateThumbnails();

    while (window.isOpen()) {
        // Music logic removed from loop (Continuous Play)

        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();

            if (const auto* resized = event->getIf<sf::Event::Resized>()) {
               sf::FloatRect visibleArea({0.f, 0.f}, {(float)resized->size.x, (float)resized->size.y});
               float windowRatio = visibleArea.size.x / visibleArea.size.y;
               float viewRatio = 800.0f / 600.0f;
               float sizeX = 1;
               float sizeY = 1;
               float posX = 0;
               float posY = 0;

               if (windowRatio > viewRatio) {
                   sizeX = viewRatio / windowRatio;
                   posX = (1 - sizeX) / 2.0f;
               } else {
                   sizeY = windowRatio / viewRatio;
                   posY = (1 - sizeY) / 2.0f;
               }

               view.setViewport(sf::FloatRect({posX, posY}, {sizeX, sizeY}));
               view.setSize({800.f, 600.f});
               view.setCenter({400.f, 300.f});
            }

           if (state == GameState::MENU_MAIN) handleMenuInput(*event);
           else if (state == GameState::MENU_LEVEL_SELECT) handleLevelSelectInput(*event);
           else if (state == GameState::PLAYING) handleGameInput(*event);
        }
        window.clear(sf::Color::Black);
        window.setView(view);

        if (state == GameState::MENU_MAIN) renderMenu();
        else if (state == GameState::MENU_LEVEL_SELECT) renderLevelSelect();
        else if (state == GameState::PLAYING) renderGame();
        else if (state == GameState::LEVEL_COMPLETE_ANIMATION) renderLevelCompleteAnimation();

        // Render Global UI (Sticky inside game view)
        window.setView(view); // [FIX] Ensure we use the game view

        window.display();
    }
}

void Game::handleMenuInput(const sf::Event& event) {
    if (const auto* mousePress = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mousePress->button == sf::Mouse::Button::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords(mousePress->position);

            // Play Input
            bool hitPlay = false;
            if (playTexture.getSize().x > 0) {
                 hitPlay = playSprite.getGlobalBounds().contains(mousePos);
            } else {
                 // Check against the visual box bounds (Center 200,480; Size 200x60; Origin Center)
                 // Left=100, Top=450, W=200, H=60
                 sf::FloatRect btnBounds({100.f, 450.f}, {200.f, 60.f});
                 hitPlay = btnBounds.contains(mousePos);
            }

            if (hitPlay) {
                state = GameState::MENU_LEVEL_SELECT;
            }

            // Exit Input
            bool hitExit = false;
            if (exitTexture.getSize().x > 0) {
                hitExit = exitSprite.getGlobalBounds().contains(mousePos);
            } else {
                 // Check against visual box bounds (Center 600,480; Size 200x60; Origin Center)
                 // Left=500, Top=450, W=200, H=60
                 sf::FloatRect btnBounds({500.f, 450.f}, {200.f, 60.f});
                 hitExit = btnBounds.contains(mousePos);
            }

            if (hitExit) {
                window.close();
            }
        }
    }
}

void Game::handleLevelSelectInput(const sf::Event& event) {
     if (const auto* mousePress = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mousePress->button == sf::Mouse::Button::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords(mousePress->position);

            // Level Selection
            for (size_t i = 0; i < levelButtons.size(); ++i) {
                if (levelButtons[i].getGlobalBounds().contains(mousePos)) {
                    currentLevelIndex = i;
                    state = GameState::PLAYING;
                    nivele[currentLevelIndex].reseteazaMinge();
                    strokes = 0;
                    isDragging = false;

                    // Level Start Sound
                    if (holeSound.getBuffer().getSampleCount() > 0) {
                        holeSound.setPitch(1.5f);
                        holeSound.play();
                    }
                }
            }

            // Back To Title Input
            if (backToTitleText.getGlobalBounds().contains(mousePos)) {
                state = GameState::MENU_MAIN;
            }
        }
     }
}

void Game::handleGameInput(const sf::Event& event) {
    if (currentLevelIndex >= nivele.size()) return;
    Level& currentLevel = nivele[currentLevelIndex];

    if (const auto* mousePress = event.getIf<sf::Event::MouseButtonPressed>()) {
         if (mousePress->button == sf::Mouse::Button::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords(mousePress->position);

             // Menu/Back Input
             bool hitBack = false;
             if (backTexture.getSize().x > 0) hitBack = backSprite.getGlobalBounds().contains(mousePos);
             else hitBack = backButtonText.getGlobalBounds().contains(mousePos);

             if (hitBack) {
                 state = GameState::MENU_LEVEL_SELECT;
                 return;
             }
             if (currentLevel.getMinge().vitezaMica()) {
                 isDragging = true;
                dragStartPos = sf::Mouse::getPosition(window);
             }
         }
    }
    if (const auto* mouseRelease = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (mouseRelease->button == sf::Mouse::Button::Left) {
            if (isDragging) {
                isDragging = false;
                sf::Vector2i dragEndPos = sf::Mouse::getPosition(window);
                sf::Vector2f startW = window.mapPixelToCoords(dragStartPos);
                sf::Vector2f endW = window.mapPixelToCoords(dragEndPos);
                sf::Vector2f deltaW = startW - endW;

                // Increased sensitivity (0.132f -> 0.35f)
                float force = std::sqrt(deltaW.x*deltaW.x + deltaW.y*deltaW.y) * 0.35f;
                float angleRad = std::atan2((float)deltaW.y, (float)deltaW.x);
                float angleDeg = angleRad * 180.0f / 3.14159f;

                if (force > 108.0f) force = 108.0f;
                // Deadzone: If drag is small (force < 5.0), cancel shot
                // 5.0f force ~= 38 pixels distance (5.0 / 0.132)
                if (force > 5.0f) {
                    currentLevel.getMinge().loveste(force, angleDeg);
                    strokes++;
                }
            }
        }
    }
}

void Game::renderMenu() {
    // Background (Keep if exists, else black)
    if (backgroundTexture.getSize().x > 0) {
        window.draw(backgroundSprite);
    }

    // Play Button
    if (playTexture.getSize().x > 0) {
        window.draw(playSprite);
    } else {
        sf::RectangleShape playBox({200.f, 60.f});
        playBox.setOrigin({100.f, 30.f});
        playBox.setPosition({200.f, 480.f});
        playBox.setFillColor(sf::Color::Black);
        playBox.setOutlineColor(sf::Color::White);
        playBox.setOutlineThickness(2.f);
        window.draw(playBox);

        playButtonText.setOrigin({playButtonText.getLocalBounds().size.x/2.f, playButtonText.getLocalBounds().size.y/2.f});
        playButtonText.setPosition({200.f, 480.f});
        window.draw(playButtonText);
    }

    // Exit Button
    if (exitTexture.getSize().x > 0) {
        window.draw(exitSprite);
    } else {
        sf::RectangleShape exitBox({200.f, 60.f});
        exitBox.setOrigin({100.f, 30.f});
        exitBox.setPosition({600.f, 480.f});
        exitBox.setFillColor(sf::Color::Black);
        exitBox.setOutlineColor(sf::Color::White);
        exitBox.setOutlineThickness(2.f);
        window.draw(exitBox);

        exitButtonText.setOrigin({exitButtonText.getLocalBounds().size.x/2.f, exitButtonText.getLocalBounds().size.y/2.f});
        exitButtonText.setPosition({600.f, 480.f});
        window.draw(exitButtonText);
    }
}

void Game::renderLevelSelect() {
    window.draw(levelSelectTitle);
    window.draw(backToTitleText); // [NEW]
    for (size_t i = 0; i < levelButtons.size(); ++i) {
        window.draw(levelButtons[i]);
        window.draw(levelButtonLabels[i]);
    }
}

void Game::renderGame() {
    if (currentLevelIndex < nivele.size()) {
        Level& currentLevel = nivele[currentLevelIndex];

        if (!isDragging) {
            for(int i=0; i<20; ++i)
                currentLevel.update(0.016f / 20.0f);

            if (currentLevel.isFinished()) {
                std::cout << "Level Complete!\n";
                // Skip first 0.5 seconds as requested
                if (holeSound.getBuffer().getSampleCount() > 0) {
                    holeSound.setPlayingOffset(sf::seconds(0.5f));
                    holeSound.play();
                }
                state = GameState::LEVEL_COMPLETE_ANIMATION;
                animationTimer = 0.0f;
                levelCompleteSoundPlayed = false; // [NEW] Reset
                currentLevel.getHole().setFlagOffset(0.0f);
                std::stringstream ss;
                ss << "Level " << (currentLevelIndex + 1) << " Complete!\n"
                   << "Strokes: " << strokes;
                levelCompleteText.setString(ss.str());
                levelCompleteText.setCharacterSize(50);
                levelCompleteText.setFillColor(sf::Color::White);
                levelCompleteText.setOutlineColor(sf::Color::Black);
                levelCompleteText.setOutlineThickness(2.0f);
                sf::FloatRect bounds = levelCompleteText.getLocalBounds();
        levelCompleteText.setOrigin({bounds.size.x/2.f, bounds.size.y/2.f});
        levelCompleteText.setPosition({400.f - bounds.size.x/2.f, 250.f});
            }
        }

        window.clear(sf::Color::Black);
        if (grassTexture.getSize().x > 0) {
            window.draw(grassSprite);
        } else {
            window.clear(sf::Color(30, 100, 30));
        }

        // Tee Rendering Removed

        currentLevel.render(window);

        // Back Button
        if (backTexture.getSize().x > 0) {
            window.draw(backSprite);
            // Optionally text?
        } else {
            window.draw(backButtonText);
        }

        // Power Meter & Arrow
        if (isDragging) {
                sf::Vector2i mousePosPix = sf::Mouse::getPosition(window);
                sf::Vector2f mousePosW = window.mapPixelToCoords(mousePosPix);
                sf::Vector2f startW = window.mapPixelToCoords(dragStartPos);

                sf::Vector2f deltaW = startW - mousePosW;
                float dist = std::sqrt(deltaW.x*deltaW.x + deltaW.y*deltaW.y);
                float force = dist * 0.35f; // Match sensitivity
                if (force > 108.0f) force = 108.0f;
                float angleRad = std::atan2(deltaW.y, deltaW.x);
                float angleDeg = angleRad * 180.0f / 3.14159f;

                Vector2D ballPos = currentLevel.getMinge().getPoz();
                sf::Vector2f ballCenter(ballPos.getX() * 10.0f, ballPos.getY() * 10.0f);

                // Meter
                float meterWidth = 10.0f;
                float meterHeight = 60.0f;
                float meterX = ballCenter.x - 30.0f;
                float meterY = ballCenter.y - meterHeight/2.0f;

                sf::RectangleShape meterBg({meterWidth, meterHeight});
                meterBg.setPosition({meterX, meterY});
                meterBg.setFillColor(sf::Color::White);
                meterBg.setOutlineColor(sf::Color::Black);
                meterBg.setOutlineThickness(1.0f);
                window.draw(meterBg);

                float fillPct = force / 108.0f;
                if (fillPct > 1.0f) fillPct = 1.0f;
                float fillHeight = meterHeight * fillPct;

                sf::RectangleShape meterFill({meterWidth, fillHeight});
                meterFill.setOrigin({0.0f, fillHeight});
                meterFill.setPosition({meterX, meterY + meterHeight});

                std::uint8_t r = static_cast<std::uint8_t>(255 * fillPct);
                std::uint8_t g = static_cast<std::uint8_t>(255 * (1.0f - fillPct));
                meterFill.setFillColor(sf::Color(r, g, 0));
                window.draw(meterFill);

                // Arrow (Hide if in deadzone)
                if (force > 5.0f) {
                    // Manual Outline Expansion Strategy

                    auto drawArrowLayer = [&](float extra, sf::Color color) {
                        sf::ConvexShape arrow;
                        arrow.setPointCount(7);

                        // Dimensions
                        float baseHeadLen = 12.0f; // Constant Head Length
                        float baseShaftLen = 15.0f + (force * 0.35f); // Growing Shaft
                        float baseWidth = 2.0f;
                        float baseHeadWidth = 7.0f;

                        // Expanded Dimensions
                        float width = baseWidth + extra;
                        float headWidth = baseHeadWidth + extra;

                        // Shaft length doesn't need extra expansion along X,
                        // just needs to match the head start.

                        // 0: Start Top
                        arrow.setPoint(0, {0.0f, -width});

                        // 1: Shaft End Top
                        arrow.setPoint(1, {baseShaftLen, -width});

                        // 2: Head Wing Top (Straight Triangle - No Sweep Back)
                        // This aligns the wing with the shaft end
                        arrow.setPoint(2, {baseShaftLen, -(headWidth)});

                        // 3: Tip
                        arrow.setPoint(3, {baseShaftLen + baseHeadLen + extra, 0.0f});

                        // 4: Head Wing Bottom
                        arrow.setPoint(4, {baseShaftLen, headWidth});

                        // 5: Shaft End Bottom
                        arrow.setPoint(5, {baseShaftLen, width});

                        // 6: Start Bottom
                        arrow.setPoint(6, {0.0f, width});

                        arrow.setFillColor(color);
                        arrow.setOutlineThickness(0);
                        arrow.setPosition(ballCenter);
                        arrow.setRotation(sf::degrees(angleDeg));

                        float offset = 10.0f;
                        arrow.move({std::cos(angleRad) * offset, std::sin(angleRad) * offset});
                        window.draw(arrow);
                    };

                    // 1. Draw Black Expanded Background (The Outline)
                    drawArrowLayer(1.5f, sf::Color::Black);

                    // 2. Draw White Normal Foreground
                    drawArrowLayer(0.0f, sf::Color::White);
                }
        }
    }
}

void Game::renderLevelCompleteAnimation() {
    animationTimer += 0.016f;

    // [NEW] Delay 0.8 second
    if (animationTimer > 0.8f && !levelCompleteSoundPlayed) {
        if (levelCompleteSound.getBuffer().getSampleCount() > 0) {
            levelCompleteSound.play();
        }
        levelCompleteSoundPlayed = true;
    }

    float progress = animationTimer / 1.5f;
    if (progress > 1.0f) progress = 1.0f;
    progress = 1.0f - std::pow(1.0f - progress, 3.0f);

    float offset = progress * 600.0f;
    nivele[currentLevelIndex].getHole().setFlagOffset(offset);

    window.clear(sf::Color::Black);
    window.setView(window.getView());
    if (grassTexture.getSize().x > 0) {
        window.draw(grassSprite);
    } else {
        window.clear(sf::Color(30, 100, 30));
    }

    Level& lvl = nivele[currentLevelIndex];
    for (const auto& o : lvl.getObstacole()) {
        o->render(window);
    }
    lvl.getHole().render(window);

    if (animationTimer > 0.5f) {
        float scale = 1.0f + std::sin(animationTimer * 5.0f) * 0.05f;
        levelCompleteText.setScale({scale, scale});
        sf::FloatRect b = levelCompleteText.getGlobalBounds();
        levelCompleteText.setOrigin({b.size.x/2.f, b.size.y/2.f});
        levelCompleteText.setPosition({400.f, 300.f});
        window.draw(levelCompleteText);
    }

    if (animationTimer > 4.0f) {
        currentLevelIndex++;
        if (currentLevelIndex >= nivele.size()) {
            state = GameState::MENU_MAIN;
        } else {
            state = GameState::PLAYING;
            nivele[currentLevelIndex].reseteazaMinge();
            strokes = 0;
            isDragging = false;
        }
    }
}

std::ostream& operator<<(std::ostream& os, const Game& /*game*/) {
    return os;
}