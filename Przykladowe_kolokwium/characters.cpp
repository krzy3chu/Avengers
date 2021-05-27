#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>

class AvengerSprite: public sf::Sprite{
protected:
    sf::Texture texture;
    int speedX, speedY;
public:
    static sf::VideoMode desktop;
    AvengerSprite(const std::string &textureName){
        texture.loadFromFile("textures/"+textureName+".png");
        setTexture(texture);
        setScale(0.1, 0.1);
    }
    void animuj(const sf::Time &elapsed){
        move(speedX * elapsed.asMicroseconds() / 1.e6, speedY * elapsed.asMicroseconds() / 1.e6);
        sf::FloatRect bounds = getGlobalBounds();
        sf::Vector2f pos = getPosition();
        if(bounds.left <= 0 && speedX < 0){     // check if object hit screen border
            setPosition(desktop.width - bounds.width, pos.y);
        }
        if(bounds.left + bounds.width >= desktop.width && speedX > 0){
            setPosition(0, pos.y);
        }
        if(bounds.top <= 0 && speedY < 0){
            setPosition(pos.x, desktop.height - bounds.height);
        }
        if(bounds.top + bounds.height >= desktop.height && speedY > 0){
            setPosition(pos.x, 0);
        }
    }
};

sf::VideoMode AvengerSprite::desktop = sf::VideoMode().getDesktopMode();

class Kamiennieskonczonosci: public AvengerSprite{
public:
    bool grow;
    Kamiennieskonczonosci(): AvengerSprite("kamien"){
        double posX, posY;
        do{
            posX = rand()% (int)(desktop.width - getGlobalBounds().width);
            posY = rand()% (int)(desktop.height - getGlobalBounds().height);
        }while(desktop.width/4 < posX && posX < desktop.width*3/4 && desktop.height/4 < posY && posY < desktop.height*3/4); //random position out of center
        setPosition(posX, posY);
        speedX = 0;
        speedY = 50;
        grow = false;
    }
    void onClick(const sf::Vector2i &mousePos){ //check if kamien was clicked
        sf::FloatRect bounds = getGlobalBounds();
        if(mousePos.x > bounds.left && mousePos.x < bounds.left + bounds.width &&
                mousePos.y > bounds.top && mousePos.y < bounds.top + bounds.height){
             grow = true;
        }
    }
    void animateGrow(const sf::Time &elapsed){ //animation of kamien growth
        if(grow && getScale().x <= 0.8){
            setScale(getScale().x * (1 + (4 * elapsed.asMicroseconds()/1.e6)), getScale().y * (1 + (4 * elapsed.asMicroseconds()/1.e6)));
        }
    }
};

class StworOutsider: public AvengerSprite{
public:
    StworOutsider(): AvengerSprite("alien"){
        double posX, posY;
        do{
            posX = rand()% (int)(desktop.width - getGlobalBounds().width);
            posY = rand()% (int)(desktop.height - getGlobalBounds().height);
        }while(desktop.width/4 < posX && posX < desktop.width*3/4 && desktop.height/4 < posY && posY < desktop.height*3/4); //random position out of center
        setPosition(posX, posY);
        speedY = 0;
        if(rand()% 2){
            speedX = 100;
        }else{
            speedX = -100;
        }
    }
};

class Thor: public AvengerSprite{
public:
    int health;
    int points;
    const int thorSpeed = 150;
    Thor(): AvengerSprite("thor"){
        setOrigin(getGlobalBounds().width/2, getGlobalBounds().height/2);
        setPosition(desktop.width/2, desktop.height/2);
        speedX = 0;
        speedY = 0;
        health = 3;
        points = 0;
    }
    void moveThor(const char &direction){
        if(direction == 'W'){
            speedY = -thorSpeed;
        }else if(direction == 'S'){
            speedY = thorSpeed;
        }else if(direction == 'I'){
            speedY = 0;
        }
        if(direction == 'A'){
            speedX = -thorSpeed;
        }else if(direction == 'D'){
            speedX = thorSpeed;
        }else if(direction == 'O'){
            speedX = 0;
        }
    }
    bool collision(const AvengerSprite &sprite){ // check if thor hit sth
        sf::FloatRect thorBounds = getGlobalBounds();
        sf::FloatRect objBounds = sprite.getGlobalBounds();
        if( ( (thorBounds.top < objBounds.top + objBounds.height && thorBounds.top > objBounds.top) ||
                        (thorBounds.top + thorBounds.height > objBounds.top && thorBounds.top < objBounds.top) ) &&
                                    ( (thorBounds.left < objBounds.left + objBounds.width && thorBounds.left > objBounds.left) ||
                                                (thorBounds.left + thorBounds.width > objBounds.left && thorBounds.left < objBounds.left) ) ){
            if((typeid(sprite)) == (typeid(StworOutsider))){
                setPosition(desktop.width/2, desktop.height/2);
                health--;
            }else{
                points += 100;
            }
            return true;
        }
        return false;
    }
};
