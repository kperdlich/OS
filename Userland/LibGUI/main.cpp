
#include "Application.h"
#include "Array.h"
#include "Rect.h"
#include "TextBox.h"
#include "Vector.h"
#include "Window.h"

int main()
{
    TEST::Vector<int> test;
    test.pushBack(10);
    test.pushBack(20);
    test.pushBack(30);
    test.pushBack(40);
    test.popBack();
    test.popBack();
    test.pushBack(24);

    for (auto it : test) {
        std::cout << it << std::endl;
    }

    for (auto it = test.rbegin(); it != test.rend(); ++it) {
        std::cout << *it << std::endl;
    }

    ADS::Array<int, 5> array = { 10, 20, 30, 40 };
    for (auto it : array) {
        std::cout << it << std::endl;
    }

    TEST::Vector<int> vector = { 10, 20, 30, 60 };
    for (auto it : vector) {
        std::cout << it << std::endl;
    }

    GUI::Button* button = new GUI::Button([]() {
        std::cout << "Button clicked" << std::endl;
    });
    button->setWindowRelativeRect(GUI::IntRect { 2, 2, 200, 30 });
    button->setText("Press me :)");

    GUI::Window win1;
    win1.setRect(GUI::IntRect { 50, 50, 600, 400 });
    win1.setCentralWidget(*button);
    win1.setTitle("Window 1");
    win1.show();

    GUI::Widget* container = new GUI::Widget();
    container->setWindowRelativeRect(GUI::IntRect { 0, 0, 300, 400 });

    GUI::TextBox* textBox = new GUI::TextBox("TextBox text");
    textBox->setWindowRelativeRect(GUI::IntRect { 2, 2, 200, 30 });
    textBox->setParent(container);

    GUI::Button* textBoxButton = new GUI::Button([&]() {
        textBox->setText("");
    });
    textBoxButton->setWindowRelativeRect(GUI::IntRect { 2, 20, 200, 30 });
    textBoxButton->setText("Reset");
    textBoxButton->setParent(container);

    GUI::Window win2;
    win2.setRect({ GUI::IntRect { 100, 100, 600, 400 } });
    win2.setTitle("Window 2");
    win2.setCentralWidget(*container);
    win2.show();

    GUI::Application app;
    return app.exec();
}
