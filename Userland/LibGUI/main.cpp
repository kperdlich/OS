
#include "Application.h"
#include "Array.h"
#include "BoxLayout.h"
#include "Terminal.h"
#include "DoublyLinkedList.h"
#include "HashMap.h"
#include "Label.h"
#include "OwnPtr.h"
#include "Rect.h"
#include "RefPtr.h"
#include "ScrollArea.h"
#include "SinglyLinkedList.h"
#include "String.h"
#include "TextBox.h"
#include "Vector.h"
#include "Window.h"

int main()
{
    /************* String tests *************/
    {
        ADS::String str;
        ASSERT(str.length() == 0);
        ASSERT(str.isEmpty());

        const ADS::String test = "test";
        ASSERT(test.length() == 4);
        ASSERT(!test.isEmpty());
        ASSERT(test != str);
        const ADS::String testCpy = test;
        ASSERT(testCpy == test);
        ASSERT(testCpy != str);

        const ADS::String splitTest = "test1 test2 test3";
        ASSERT(splitTest == "test1 test2 test3");
        const auto splits = splitTest.split(' ');
        ASSERT(splits.size() == 3);
        ASSERT(splits[0] == "test1");
        ASSERT(splits[1] == "test2");
        ASSERT(splits[2] == "test3");

        ADS::String concatStr = "ABC";
        concatStr.append(" DEF");
        ASSERT(concatStr == "ABC DEF");
        concatStr.appendChar('G');
        ASSERT(concatStr == "ABC DEFG");
        const ADS::String additionalString = " HIJK";
        concatStr.append(additionalString);
        ASSERT(concatStr == "ABC DEFG HIJK");

        {
            const ADS::String str = "ABCDEF";
            ASSERT(str.substring(0) == "ABCDEF");
            ASSERT(str.substring(1, 3) == "BCDE");
            ASSERT(str.substring(1, 20) == "BCDEF");
        }

        {
            ADS::String str = "ABCDEF";
            ASSERT(str.remove(2, 1) == "ABDEF");
            ASSERT(str.remove(0, 1) == "BDEF");
            ASSERT(str.remove(1, 10) == "B");
            const ADS::String copy = str;
            ASSERT(copy == "B");
        }

        {
            ADS::String str = "ABCDEF";
            ASSERT(str.insert(0, "1") == "1ABCDEF");
            ASSERT(str.insert(1, "23") == "123ABCDEF");
            ASSERT(str.insert(1, "A") == "1A23ABCDEF");
            ASSERT(str.insert(str.length(), "-XXX") == "1A23ABCDEF-XXX");
            ASSERT(str.insert(3, "ZZZ") == "1A2ZZZ3ABCDEF-XXX");
        }

        {
            ADS::String intStr = "1230";
            int intValue {};
            ASSERT(intStr.toInt(intValue));
            ASSERT(intValue == 1230);

            ADS::String floatStr = "1230.3";
            float floatValue {};
            ASSERT(floatStr.toFloat(floatValue));
            ASSERT(floatValue == 1230.3f);

            ADS::String doubleStr = "1230.30";
            double doubleValue {};
            ASSERT(doubleStr.toDouble(doubleValue));
            ASSERT(doubleValue == 1230.30);

            //ADS::String fromFloatValue;
            //fromFloatValue.fromFloat(floatValue);
            //ASSERT(fromFloatValue == "1230.3");
        }

        {
            const ADS::String test = "test1";
            ASSERT(test.startsWith("test"));
            ASSERT(!test.startsWith("test123"));
            ASSERT(!test.startsWith(""));
            ASSERT(test.startsWith("test1"));
        }

        {
            const ADS::String test = "test1";
            ASSERT(test.endsWith("test1"));
            ASSERT(test.endsWith("t1"));
            ASSERT(!test.endsWith("ttest1"));
            ASSERT(!test.endsWith(""));
        }

        {
            ADS::String test = "aB!cD";
            ASSERT(test.toUpper() == "AB!CD");
        }

        {
            ADS::String test = "aB!cD";
            ASSERT(test.toLower() == "ab!cd");
        }
    }

    std::cout << "Rect to string: " << GUI::Rect { 100, 100, 20, 20}.toString() << std::endl;

    /************* Function tests *************/
    {
        constexpr const int expectedIntValue = 10;

        ADS::Function<int(int)> fx = [expectedIntValue](int x) -> int {
            ASSERT(x == expectedIntValue);
            return x;
        };
        ASSERT(fx(expectedIntValue) == expectedIntValue);

        ADS::Function<int(int)> movedFx = ADS::move(fx);
        ASSERT(movedFx(expectedIntValue) == expectedIntValue);
    }
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

    /************* DoublyLinkedList tests *************/
    {
        ADS::DoublyLinkedList<int> emptyList;
        ASSERT(emptyList.isEmpty());
        for (auto item : emptyList) {
            ASSERT(false);
        }
    }

    {
        ADS::DoublyLinkedList<int> filledList;
        int test = 20;
        filledList.add(10);
        filledList.add(30);
        filledList.add(ADS::move(test));
        ASSERT(!filledList.isEmpty());
        ASSERT(filledList.size() == 3);
        for (auto item : filledList) {
            std::cout << item << std::endl;
        }
        auto iterator = filledList.find(30);
        ASSERT(iterator != filledList.end());
        ASSERT(*iterator == 30);
        ASSERT(*(--iterator) == 10);
        ASSERT(filledList.remove(iterator));
        ASSERT(filledList.size() == 2);
        ASSERT(!filledList.remove(filledList.end()));
        ASSERT(filledList.remove(filledList.begin()));
        ASSERT(filledList.size() == 1);
        ASSERT(filledList.find(20) != filledList.end());
        for (auto item : filledList) {
            ASSERT(item == 20);
        }
        filledList.clear();
        ASSERT(filledList.size() == 0);
        ASSERT(filledList.isEmpty());
    }

    /************* HashMap tests *************/
    {
        ADS::HashMap<int, int> map;
        for (auto& entry : map)
            ASSERT(false);
        ASSERT(map.isEmpty());
        map.set(1, 10);
        ASSERT(!map.isEmpty());
        ASSERT(map.contains(1));
        ASSERT(!map.contains(2));
        ASSERT(map.getValueOrDefault(1, -1) == 10);
        ASSERT(map.getValueOrDefault(2, -1) == -1);
        {
            int test;
            ASSERT(map.tryGetValue(1, test));
            ASSERT(test == 10);
            ASSERT(!map.tryGetValue(2, test));
        }

        {
            map.set(10, 20);
            ASSERT(map.contains(10));
            ASSERT(map.size() == 2);
            ASSERT(map.remove(10) == 1);
            ASSERT(map.remove(100) == 0);
            ASSERT(map.size() == 1);
        }
        // Rehash tests
        {
            map.clear();
            ASSERT(map.isEmpty());
            map.set(1, 20);
            map.set(2, 20);
            map.set(3, 20);
            map.set(4, 20);
            map.set(5, 20);
            map.set(6, 20);

            ASSERT(map.contains(1));
            ASSERT(map.contains(2));
            ASSERT(map.contains(3));
            ASSERT(map.contains(4));
            ASSERT(map.contains(5));
            ASSERT(map.contains(6));

            std::cout << "HashMap iterator:" << std::endl;
            for (auto& entry : map)
                std::cout << "[" << entry.key << " -> " << entry.value << "]" << std::endl;

            const ADS::HashMap<int, int> constMap = ADS::move(map);
            std::cout << "HashMap ConstIterator:" << std::endl;
            for (const auto& entry : constMap)
                std::cout << "[" << entry.key << " -> " << entry.value << "]" << std::endl;

            std::cout << "HashMap: " << map.toString() << std::endl;
        }
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

    {
        GUI::Terminal* terminalWidget = new GUI::Terminal();
        GUI::Window* terminalWindow = new GUI::Window();
        terminalWindow->setTitle("Terminal");
        terminalWindow->setCentralWidget(*terminalWidget);
        terminalWindow->show();
    }

    return app.exec();
}
