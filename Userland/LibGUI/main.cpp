
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
        GUI::Widget* buttonListLayoutWidget = new GUI::Widget();
        GUI::VBoxLayout* buttonList = new GUI::VBoxLayout(buttonListLayoutWidget);
        buttonList->setSpacing(10);
        buttonList->addWidget(*new GUI::Button("Btn1"));
        buttonList->addWidget(*new GUI::Button("Btn2"));
        buttonList->addWidget(*new GUI::Button("Btn3"));
        buttonList->addWidget(*new GUI::Button("Btn4"));
        buttonList->addWidget(*new GUI::Button("Btn5"));
        buttonListLayoutWidget->setLayout(buttonList);

        GUI::ScrollArea* scrollArea = new GUI::ScrollArea();
        scrollArea->setWidget(buttonListLayoutWidget);

        GUI::Widget* rootWidget = new GUI::Widget();
        GUI::VBoxLayout* rootLayout = new GUI::VBoxLayout(rootWidget);
        GUI::Button* button = new GUI::Button(rootWidget);
        button->setText("Press me :)");
        button->onClick = []() {
            std::cout << "Button clicked" << std::endl;
        };
        rootLayout->addWidget(*button);
        rootLayout->addWidget(*scrollArea);
        rootWidget->setLayout(rootLayout);

        GUI::Window* win1 = new GUI::Window();
        win1->setRect({300, 300, 640, 480});
        win1->setCentralWidget(*rootWidget);
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
        formLayout->setSpacing(10);
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

        containerLayout->addWidget(*textBoxButton);
        containerLayout->addWidget(*formWidget);
        containerLayout->addWidget(*new GUI::TextBox("Test"));

        GUI::Window* win2 = new GUI::Window();
        win2->setTitle("Window 2");
        win2->setCentralWidget(*root);
        win2->show();
    }

    return app.exec();
}
