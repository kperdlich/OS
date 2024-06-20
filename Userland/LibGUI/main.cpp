
#include "Application.h"
#include "Array.h"
#include "BoxLayout.h"
#include "Label.h"
#include "Rect.h"
#include "ScrollArea.h"
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
        button->setText("Press me :)");

        GUI::ScrollArea* scrollArea = new GUI::ScrollArea();
        scrollArea->setWidget(*button);
        scrollArea->setContentSize({200, 200});

        GUI::Window* win1 = new GUI::Window();
        win1->setCentralWidget(*scrollArea);
        win1->setTitle("Window 1");
        win1->show();
    }

    {

        GUI::BoxLayout* containerLayout = new GUI::BoxLayout(GUI::BoxLayout::Direction::Vertical);
        containerLayout->setSpacing(10);

        GUI::Widget* root = new GUI::Widget();
        root->setLayout(containerLayout);

        GUI::Widget* formWidget = new GUI::Widget(root);
        GUI::BoxLayout* formLayout = new GUI::BoxLayout(GUI::BoxLayout::Direction::Horizontal);
        formWidget->setLayout(formLayout);

        GUI::TextBox* textBox = new GUI::TextBox("TextBox");

        GUI::Label* label = new GUI::Label("Label");
        label->setAlignment(GUI::Alignment::Center);
        label->shrinkToFit();

        GUI::Button* textBoxButton = new GUI::Button(
            [&]() {
                textBox->setText("");
            },
            root);
        textBoxButton->setText("Reset");

        //// Layout stuff
        formLayout->addWidget(*label);
        formLayout->addWidget(*textBox);

        containerLayout->addWidget(*formWidget);
        containerLayout->addWidget(*textBoxButton);
        containerLayout->addWidget(*new GUI::TextBox("Test"));

        GUI::Window* win2 = new GUI::Window();
        win2->setTitle("Window 2");
        win2->setCentralWidget(*root);
        win2->show();
    }

    return app.exec();
}
