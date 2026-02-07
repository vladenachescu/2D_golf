#pragma once
#include "Level.h"
#include <iostream>
#include <vector> // <--- NECESAR PENTRU VECTOR
#include <SFML/Graphics.hpp>


#include <SFML/Audio.hpp> // Added for Music

enum class GameState {
    MENU_MAIN,
    MENU_LEVEL_SELECT,
    PLAYING,
    LEVEL_COMPLETE_ANIMATION // [NEW]
};

class Game {
private:
    std::vector<Level> nivele;
    sf::RenderWindow window;
    
    // State
    GameState state;
    std::size_t currentLevelIndex;
    int scor;
    int strokes = 0; // [NEW]
    float animationTimer = 0.0f; // [NEW]
    
    // Gameplay
    bool isDragging;
    sf::Vector2i dragStartPos;

    // UI
    sf::Font font;
    sf::Text titleText;
    sf::Text playButtonText;
    sf::Text exitButtonText;
    sf::Text backButtonText; // For in-game menu return
    sf::Text backToTitleText; // [NEW] From Level Select to Main
    
    sf::Text levelSelectTitle;
    sf::Text levelCompleteText; // [NEW]
    
    // Level Select
    std::vector<sf::RectangleShape> levelButtons;
    std::vector<sf::Text> levelButtonLabels;
    std::vector<sf::Texture> levelThumbnails;
    std::vector<sf::Sprite> levelThumbnailSprites;

    
    // Texture Placeholders (Assets)
    sf::Texture playTexture;  // [NEW]
    sf::Sprite playSprite;    // [NEW]
    sf::Texture exitTexture;  // [NEW]
    sf::Sprite exitSprite;    // [NEW]
    sf::Texture backTexture;  // [NEW]
    sf::Sprite backSprite;    // [NEW]
    sf::Texture levelBtnTexture; // [NEW]
    sf::Texture backgroundTexture; // [NEW]
    sf::Sprite backgroundSprite;   // [NEW]
    
    sf::Texture wallTexture;
    sf::Texture waterTexture;
    sf::Texture sandTexture;
    sf::Texture holeTexture;
    sf::Texture grassTexture;
    sf::Texture ballTexture; 
    sf::Sprite grassSprite;
    
    // Music removed
    
    // SFX
    sf::SoundBuffer holeSoundBuffer;
    sf::Sound holeSound;
    
    sf::SoundBuffer levelCompleteBuffer; // [NEW]
    sf::Sound levelCompleteSound;       // [NEW]
    bool levelCompleteSoundPlayed = false; // [NEW]

    // Helper methods
    void initLevels();
    void initUI();
    void generateThumbnails();
    void handleMenuInput(const sf::Event& event);
    void handleLevelSelectInput(const sf::Event& event);
    void handleGameInput(const sf::Event& event);
    
    void renderMenu();
    void renderLevelSelect();
    void renderGame();
    void renderLevelCompleteAnimation(); // [NEW]

public:
    Game();
    void start();
    // Removed specific methods that are now internal logic
    // void adaugaNivel...
    // void afiseazaScor...
    
    friend std::ostream& operator<<(std::ostream& os, const Game& game);
};