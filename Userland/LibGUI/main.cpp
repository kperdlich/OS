
#include "Application.h"
#include "Array.h"
#include "BoxLayout.h"
#include "Label.h"
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

    GUI::Application app;

    {
        GUI::Button* button = new GUI::Button([]() {
            std::cout << "Button clicked" << std::endl;
        });
        button->setWindowRelativeRect(GUI::Rect { 0, 0, 200, 30 });
        button->setText("Press me :)");

        GUI::Window* win1 = new GUI::Window();
        win1->setRect(GUI::Rect { 50, 50, 600, 400 });
        win1->setCentralWidget(*button);
        win1->setTitle("Window 1");
        win1->show();
    }

    {
        GUI::Widget* container = new GUI::Widget();
        container->setWindowRelativeRect(GUI::Rect { 0, 0, 300, 400 });

        GUI::BoxLayout* layout = new GUI::BoxLayout(GUI::BoxLayout::Direction::Vertical);
        layout->setSpacing(10);

        container->setLayout(layout);
        GUI::TextBox* textBox = new GUI::TextBox("TextBox");
        textBox->setWindowRelativeRect(GUI::Rect { 0, 0, 200, 30 });
        textBox->setParent(container);

        GUI::Button* textBoxButton = new GUI::Button([&]() {
            textBox->setText("");
        });
        textBoxButton->setWindowRelativeRect(GUI::Rect { 0, textBox->windowRelativeRect().y() + textBox->windowRelativeRect().height(), 200, 30 });
        textBoxButton->setText("Reset");
        textBoxButton->setParent(container);

        GUI::Label* label = new GUI::Label(container);
        label->setText("Label");
        label->setWindowRelativeRect(GUI::Rect { 0, textBoxButton->windowRelativeRect().y() + textBoxButton->windowRelativeRect().height(), 50, 30 });

        //// Layout stuff
        layout->addWidget(*textBox);
        layout->addWidget(*textBoxButton);
        layout->addWidget(*label);

        GUI::Window* win2 = new GUI::Window();
        win2->setRect({ GUI::Rect { 100, 100, 600, 400 } });
        win2->setTitle("Window 2");
        win2->setCentralWidget(*container);
        win2->show();
    }

    return app.exec();
}
