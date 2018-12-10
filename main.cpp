#include <SFML/Graphics.hpp>
#include <iostream>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Sound.hpp>
#include <sstream>

using namespace sf;
using namespace std;

constexpr int windowHeight = 500;
constexpr int borderRight = 700;
constexpr int borderLeft = 0;
constexpr int heroHeight = 50;
constexpr int heroWidth = 40;
constexpr int ground = windowHeight - heroHeight;


float offsetX = 0, offsetY = 0;

const int H = 12;
const int W = 40;

String TileMap[H] = {

"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
"B                                B     B",
"B                                B     B",
"B                                B     B",
"B                                B     B",
"B         0000                BBBB     B",
"B                                B     B",
"BBB                              B     B",
"B              BB                BB    B",
"B              BB         BB           B",
"B    B         BB         BB           B",
"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",

};

class Player
{
public:
    float dx , dy;
    FloatRect rect;
    bool onGround;
    Sprite sprite;
    float currentFrame;

    Player(Texture& image)
    {
        sprite.setTexture(image);
        rect =  FloatRect(5*32, 9*32, 40, 50);
        dx = dy = 0.1;
        currentFrame = 0;
    }

    void update(float time)
    {



        //Vector2f v = sprite.getPosition();
        //cout << (rect.left + dx*time) << endl;
        //if ((rect.left + dx*time) >= float(borderLeft) && (rect.left + dx*time + heroWidth) <= float(borderRight) )
        {
            //cout << "Too left" << endl;
            rect.left += dx*time;
        }
        //cout << rect.left << " " << borderLeft << " " << borderRight << endl;

        Collision(0);

        if (!onGround)
            dy = dy + 0.0005*time;
        rect.top += dy*time;

        onGround = false;

        Collision(1);

        /*if (rect.top > ground)
        {
            rect.top = ground;
            dy = 0;
            onGround = true;
        }*/

        currentFrame += 0.005*time;
        if (currentFrame > 6)
            currentFrame -= 6;



        if (dx > 0/* && v.x < borderLeft*/)
            sprite.setTextureRect(IntRect(40*int(currentFrame), 244, 40, 50));
        if (dx < 0/* && v.x > borderLeft*/)
            sprite.setTextureRect(IntRect(40*int(currentFrame)+40, 244, -40, 50));

        sprite.setPosition(rect.left - offsetX, rect.top - offsetY);


        dx = 0;
    }

    void Collision(int dir)
    {
        for (int i = rect.top/32 ; i<(rect.top+rect.height)/32; i++)
            for (int j = rect.left/32; j<(rect.left+rect.width)/32; j++)
            {
                if (TileMap[i][j]=='B')
                {
                    // Right Border
                    if ((dx>0) && (dir==0))
                    {
                        cout << "Right-border" << rect.left << "; rect.wid" << rect.width;
                        rect.left =  j*32 -  rect.width;
                        cout << "; J=" << j << ";" << rect.left << endl;
                    }
                    // Left Border
                    if ((dx<0) && (dir==0))
                    {
                        cout << "Left-border" << rect.left;
                        rect.left =  j*32 + 32;
                        cout << "; J=" << j << ";" << rect.left << endl;
                    }
                    if ((dy>0) && (dir==1))  { rect.top =   i*32 -  rect.height;  dy=0;   onGround=true; }
                    if ((dy<0) && (dir==1))  { rect.top = i*32 + 32;   dy=0;}
                }
                if (TileMap[i][j]=='0')
                {
                    TileMap[i][j]=' ';
                }
            }

    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(borderRight, windowHeight), "SFML works!");



    float  currentFrame = 0;

    /*sf::Sprite s;
    s.setTexture(t);
    s.setTextureRect(IntRect(0, 244, 40, 50));
    s.setPosition(50, 100);*/


    sf::Texture t;
    t.loadFromFile("fang.png");
    Player p(t );

    Clock clock;
    RectangleShape rectangle(Vector2f(32, 32));

    SoundBuffer buffer;
    if (!buffer.loadFromFile("Jump.ogg"))
    {
        return -1;
    }

    sf::Sound sound(buffer);


    Music music;
    music.openFromFile("Mario_Theme.ogg");
    music.play();



    while (window.isOpen())
    {
        double time  = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time = time / 500;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }


        if (Keyboard::isKeyPressed(Keyboard::Left))
        {

            p.dx = -0.1;
        }

        if (Keyboard::isKeyPressed(Keyboard::Right))
        {
            p.dx = 0.1;
        }

        if (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::Space))
        {
            if (p.onGround)
            {
                p.dy = -0.4;
                p.onGround = false;
                sound.play();
            }
        }

        p.update(time);

        if (p.rect.left > borderRight/2)
            offsetX = p.rect.left - borderRight/2;
        if (p.rect.top > windowHeight/2)
            offsetY = p.rect.top - windowHeight/2;
        window.clear(Color::White);

        for (int i = 0; i < H; i++)
        {
            for (int j = 0; j < W; j++)
            {
                if (TileMap[i][j] == 'B')
                    rectangle.setFillColor(Color::Black);
                if (TileMap[i][j] == '0')
                    rectangle.setFillColor(Color::Green);
                if (TileMap[i][j] == ' ')
                    continue;

                rectangle.setPosition(j*32 - offsetX, i*32 - offsetY);
                window.draw(rectangle);
            }
        }

        window.draw(p.sprite);
        window.display();
    }

    return 0;
}
