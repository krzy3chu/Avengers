#include "characters.cpp"
#include <ctime>
#include <iostream>


int main(){
    srand(time(NULL));
    sf::VideoMode desktop = sf::VideoMode().getDesktopMode();
    sf::RenderWindow window(desktop, "Avengers", sf::Style::None);

    Thor thor;
    std::vector<std::unique_ptr<AvengerSprite>> objects;
    for(int i = 0; i < 20; i++){ // spawn 20 Outsiders
        auto stwor = std::make_unique<StworOutsider>();
        objects.emplace_back(move(stwor));
    }
    for(int i = 0; i < 6; i++){ // spawn 6 Stones
        auto kamien = std::make_unique<Kamiennieskonczonosci>();
        objects.emplace_back(move(kamien));
    }

    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if(event.type == sf::Event::Closed){
                window.close();
            }
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            window.close();
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            thor.moveThor('W');
        }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            thor.moveThor('S');
        }else{
            thor.moveThor('I'); // comment this line if you want to make Thor unstoppable
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            thor.moveThor('A');
        }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            thor.moveThor('D');
        }else{
            thor.moveThor('O'); // comment this line if you want to make Thor unstoppable
        }
        if(event.type == sf::Event::MouseButtonPressed) {
            if(event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                for(auto &obj: objects){
                    Kamiennieskonczonosci *kamien = dynamic_cast<Kamiennieskonczonosci *>(obj.get());
                    if(kamien != nullptr){
                        kamien->onClick(mousePos);
                    }
                }
            }
        }
        sf::Time elapsed = clock.restart();
        window.clear(sf::Color::Black);
        thor.animuj(elapsed);
        window.draw(thor);
        for(auto it = objects.begin(); it < objects.end(); it++){
            (*it)->animuj(elapsed);
            Kamiennieskonczonosci *kamien = dynamic_cast<Kamiennieskonczonosci *>((*it).get());
            if(kamien != nullptr){
                kamien->animateGrow(elapsed);
            }
            window.draw(*(*it));
            if(thor.collision(*(*it))){
                objects.erase(it);
                it--;
            }
        }
        window.display();
        std::cout << "Health: " << thor.health << "\t Points: " << thor.points << std::endl;
        if(thor.health == 0){
            std::cout << "Przegrana :(" << std::endl;
            break;
        }
        if(thor.points == 600){
            std::cout << "Wygrana! :)" << std::endl;
            break;
        }
    }
    std::cout << "Nacisnij escape aby zakonczyc" << std::endl;
    while(window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if(event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
                window.close();
            }
        }
    }
}
