
#include "Application.h"
#include "Array.h"
#include "BoxLayout.h"
#include "Label.h"
#include "OwnPtr.h"
#include "Rect.h"
#include "RefPtr.h"
#include "ScrollArea.h"
#include "SinglyLinkedList.h"
#include "TextBox.h"
#include "Vector.h"
#include "Window.h"

int main()
{
    /************* OwnPtr tests *************/
    {
        ADS::OwnPtr<int> ownPtrDefault;
        ADS::OwnPtr<int> ownPtrEmpty = nullptr;
        ASSERT(ownPtrEmpty.ptr() == nullptr);

        ADS::OwnPtr<int> ownPtr = ADS::makeOwn<int>(13);
        ASSERT(*ownPtr == 13);

        ownPtrEmpty = ADS::move(ownPtr);
        ASSERT(*ownPtrEmpty == 13);
        ASSERT(ownPtr.ptr() == nullptr);

        ownPtrEmpty.clear();
        ASSERT(ownPtrEmpty.ptr() == nullptr);
    }

    /************* RefPtr tests *************/
    {
        ADS::RefPtr<int> refPtrDefault = nullptr;
        ADS::RefPtr<int> refPtr = nullptr;
        ASSERT(refPtr.refCount() == 0);
        ASSERT(refPtrDefault.refCount() == 0);

        ADS::RefPtr<int> refPtrInt = ADS::makeRef<int>(2);
        ASSERT(*refPtrInt == 2);
        ASSERT(refPtrInt.refCount() == 1);
        {
            ADS::RefPtr<int> refPtrIntCopy = refPtrInt;
            ASSERT(*refPtrIntCopy == 2);
            ASSERT(refPtrIntCopy.refCount() == 2);
            ASSERT(refPtrInt == refPtrIntCopy);
        }
        ASSERT(refPtrInt.refCount() == 1);
    }

    /************* WeakPtr tests *************/
    {
        ADS::WeakPtr<int> weakPtrDefault;
        ASSERT(weakPtrDefault.refCount() == 0);
        ASSERT(weakPtrDefault.isNull());

        ADS::WeakPtr<int> weakPtrNull = nullptr;
        ASSERT(weakPtrNull.refCount() == 0);
        ASSERT(weakPtrNull.isNull());

        ADS::RefPtr<int> refPtrEmpty = nullptr;
        ADS::WeakPtr<int> weakPtrEmptyRefPtr = refPtrEmpty;
        ASSERT(weakPtrEmptyRefPtr.refCount() == 0);
        ASSERT(weakPtrEmptyRefPtr.isNull());

        ADS::RefPtr<int> refPtrInt = ADS::makeRef<int>(30);
        ADS::WeakPtr<int> weakPtrInt = refPtrInt;
        ASSERT(weakPtrInt.refCount() == 1);
        ASSERT(!weakPtrInt.isNull());
        {
            if (ADS::RefPtr scopedRefPtrInt = weakPtrInt.lock()) {
                ASSERT(scopedRefPtrInt.refCount() == 2);
                ASSERT(refPtrInt.refCount() == 2);
                ASSERT(weakPtrInt.refCount() == 2);
                ASSERT(*scopedRefPtrInt == 30);
            } else {
                ASSERT(false);
            }
            ASSERT(refPtrInt.refCount() == 1);
        }

        ASSERT(weakPtrInt.refCount() == 1);
        ASSERT(!weakPtrInt.isNull());
        weakPtrInt.clear();
        ASSERT(weakPtrInt.isNull());
        ASSERT(weakPtrInt.refCount() == 0);

        ADS::RefPtr emptyRefPtr = weakPtrInt.lock();
        ASSERT(emptyRefPtr == nullptr);
    }

    /************* SinglyLinkedList tests *************/
    {
        ADS::SinglyLinkedList<int> emptyList;
        ASSERT(emptyList.isEmpty());
        for (auto item : emptyList) {
            ASSERT(false);
        }
    }
    {
        ADS::SinglyLinkedList<int> filledList;
        int test = 20;
        filledList.add(10);
        filledList.add(30);
        filledList.add(ADS::move(test));
        ASSERT(!filledList.isEmpty());
        std::cout << "/************* SinglyLinkedList tests *************/" << std::endl;
        for (auto item : filledList) {
            std::cout << item << std::endl;
        }
        auto iterator = filledList.find(30);
        ASSERT(iterator != filledList.end());
        ASSERT(*iterator == 30);
        ASSERT(filledList.remove(iterator));
        ASSERT(!filledList.remove(filledList.end()));
        ASSERT(filledList.remove(filledList.begin()));
        ASSERT(filledList.find(20) != filledList.end());
        for (auto item : filledList) {
            ASSERT(item == 20);
        }
        filledList.clear();
        ASSERT(filledList.isEmpty());
    }

    /************* Vector tests *************/
    std::cout << "/************* Vector tests *************/" << std::endl;
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

    /************* LibGUI tests *************/
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
        button->setText("Resize and translate");
        rootLayout->addWidget(*button);
        rootLayout->addWidget(*scrollArea);
        rootWidget->setLayout(rootLayout);

        GUI::Window* win1 = new GUI::Window();
        win1->resize(640, 480);
        win1->setCentralWidget(*rootWidget);
        win1->setTitle("Window 1");
        button->onClick = [win1]() {
            std::cout << "Button clicked" << std::endl;
            win1->resize(720, 480);
            win1->setPosition(100, 100);
        };
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
