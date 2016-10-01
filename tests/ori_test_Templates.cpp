#include "../testing/OriTestBase.h"
#include "../core/OriTemplates.h"

namespace Ori {
namespace Test {
namespace TemplatesTests {

////////////////////////////////////////////////////////////////////////////////

class TestSingleton : public Singleton<TestSingleton>
{
public:
    bool constructed = true;
};

TEST_METHOD(singleton)
{
    auto ptr = TestSingleton::instancePtr();
    ASSERT_IS_TRUE(ptr->constructed)

    ptr->constructed = false;

    auto ref = TestSingleton::instance();
    ASSERT_IS_FALSE(ref.constructed)
}

////////////////////////////////////////////////////////////////////////////////

class TestListener
{
public:
    void method() { notified = true; }
    bool notified = false;
};

class TestNotifier : public Notifier<TestListener>
{
public:
    void notify() { NOTIFY_LISTENERS(method); }
};

TEST_METHOD(notifier)
{
    TestListener listener;
    TestNotifier notifier;

    listener.notified = false;
    notifier.registerListener(&listener);
    notifier.notify();
    ASSERT_IS_TRUE(listener.notified)

    listener.notified = false;
    notifier.unregisterListener(&listener);
    notifier.notify();
    ASSERT_IS_FALSE(listener.notified)
}

////////////////////////////////////////////////////////////////////////////////

DECLARE_ENUM(TestEnum, 145, TestEnum_1, TestEnum_2, TestEnum_3)

TEST_METHOD(declare_enum)
{
    ASSERT_EQ_INT(ENUM_COUNT(TestEnum), 3)
    ASSERT_EQ_INT(ENUM_MIN(TestEnum), 145)
    ASSERT_EQ_INT(ENUM_MAX(TestEnum), 147)

    auto values = ENUM_VALUES(TestEnum);
    ASSERT_EQ_INT(values.size(), 3)
    ASSERT_EQ_INT(values[0], TestEnum_1)
    ASSERT_EQ_INT(values[1], TestEnum_2)
    ASSERT_EQ_INT(values[2], TestEnum_3)

    auto names = ENUM_NAMES(TestEnum);
    for (auto n : names) TEST_LOG(n)
    ASSERT_EQ_STR(names.size(), 3)
    ASSERT_EQ_STR(names[0], "TestEnum_1")
    ASSERT_EQ_STR(names[1], "TestEnum_2")
    ASSERT_EQ_STR(names[2], "TestEnum_3")

    ASSERT_EQ_STR(ENUM_ITEM_NAME(TestEnum, TestEnum_1), "TestEnum_1")
    ASSERT_EQ_STR(ENUM_ITEM_NAME(TestEnum, TestEnum_2), "TestEnum_2")
    ASSERT_EQ_STR(ENUM_ITEM_NAME(TestEnum, TestEnum_3), "TestEnum_3")

    bool ok;
    auto val = ENUM_ITEM_BY_NAME(TestEnum, "TestEnum_2", &ok);
    ASSERT_IS_TRUE(ok)
    ASSERT_EQ_INT(val, TestEnum_2);

    val = ENUM_ITEM_BY_NAME(TestEnum, "TestEnum2", &ok);
    ASSERT_IS_FALSE(ok)
    ASSERT_EQ_INT(val, TestEnum_1)
}

////////////////////////////////////////////////////////////////////////////////

TEST_METHOD(breakable_block)
{
    int counter1 = 0;
    int counter2 = 0;
    BREAKABLE_BLOCK
    {
        counter1++;
        break;
        counter2++;
    }
    ASSERT_EQ_INT(counter1, 1)
    ASSERT_EQ_INT(counter2, 0)
}

TEST_METHOD(nested_breakable_block)
{
    int counter1 = 0;
    int counter2 = 0;
    int counter3 = 0;
    int counter4 = 0;
    BREAKABLE_BLOCK
    {
        counter3++;
        counter4++;
        BREAKABLE_BLOCK
        {
            counter1++;
            break;
            counter2++;
        }
        counter3++;
        break;
        counter4++;
    }
    ASSERT_EQ_INT(counter1, 1)
    ASSERT_EQ_INT(counter2, 0)
    ASSERT_EQ_INT(counter3, 2)
    ASSERT_EQ_INT(counter4, 1)
}

////////////////////////////////////////////////////////////////////////////////

TEST_GROUP("Templates",
    ADD_TEST(singleton),
    ADD_TEST(notifier),
    ADD_TEST(declare_enum),
    ADD_TEST(breakable_block),
    ADD_TEST(nested_breakable_block),
)

} // namespace TemplatesTests
} // namespace Test
} // namespace Ori
