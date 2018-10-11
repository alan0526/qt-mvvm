#include <gtest/gtest.h>
#include "sessionmodel.h"
#include "jsonmodel.h"
#include "sessionitem.h"
#include "test_utils.h"
#include <QJsonObject>
#include <QJsonArray>

//! Checks JsonModel class and its ability to convert SessionModel to json and back.

class TestJsonModel : public ::testing::Test
{
public:
    ~TestJsonModel();

    static const QString test_dir;

    static void SetUpTestCase()
    {
        TestUtils::CreateTestDirectory(test_dir);
    }
};

TestJsonModel::~TestJsonModel() = default;
const QString TestJsonModel::test_dir = "test_JsonModel";

//! Checks the validity of json object representing item tree.

TEST_F(TestJsonModel, isValidItem)
{
    JsonModel converter;

    // empty json object is not valid
    QJsonObject object;
    EXPECT_FALSE(converter.is_item(object));

    // it also should contain array
    object[JsonModel::modelKey] = "abc";
    object[JsonModel::itemsKey] = 42; // incorrect
    object[JsonModel::itemDataKey] = QJsonArray();
    EXPECT_FALSE(converter.is_item(object));

    // correctly constructed
    object[JsonModel::itemsKey] = QJsonArray();
    object[JsonModel::itemDataKey] = QJsonArray();
    EXPECT_TRUE(converter.is_item(object));

    // wrong extra key in json
    object["abc"] = "abc";
    EXPECT_FALSE(converter.is_item(object));
    EXPECT_FALSE(converter.is_item(object));
}

//! Checks creation of json object: single item (no children) without the data.

TEST_F(TestJsonModel, singleItem)
{
    const QString model_type("MultiLayer");

    JsonModel converter;
    std::unique_ptr<SessionItem> parent(new SessionItem(model_type.toStdString()));

    QJsonObject object;
    converter.item_to_json(parent.get(), object);

    EXPECT_EQ(object[JsonModel::modelKey], model_type);
    EXPECT_EQ(object[JsonModel::itemsKey].toArray().size(), 0);
    EXPECT_EQ(object[JsonModel::itemDataKey].toArray().size(), 0);
}

//! Checks creation of json object: parent item with one data variant and one child on board.

TEST_F(TestJsonModel, parentAndChild)
{
    const QString model_type("MultiLayer");
    JsonModel converter;

    // constructing multilayer
    std::unique_ptr<SessionItem> parent(new SessionItem(model_type.toStdString()));
    parent->setData(QVariant::fromValue(42), 1);
    auto child = new SessionItem("Layer");
    parent->insertItem(-1, child);

    QJsonObject object;
    converter.item_to_json(parent.get(), object);

    EXPECT_EQ(object[JsonModel::modelKey], model_type);
    EXPECT_EQ(object[JsonModel::itemsKey].toArray().size(), 1);
    EXPECT_EQ(object[JsonModel::itemDataKey].toArray().size(), 1);

    // saving to file
    auto fileName = TestUtils::TestFileName(test_dir, "items.json");
    TestUtils::SaveJson(object, fileName);
}

//! Validity of json object represention SessionModel.

TEST_F(TestJsonModel, isValidModel)
{
    JsonModel converter;

    // empty json object is not valid
    QJsonObject object;
    EXPECT_FALSE(converter.is_model(object));

    // jsob object representing SessionItem can not represent the model
    object[JsonModel::modelKey] = "abc";
    object[JsonModel::itemsKey] = QJsonArray();
    object[JsonModel::itemDataKey] = QJsonArray();
    EXPECT_FALSE(converter.is_model(object));

    QJsonObject object2;
    object2[JsonModel::modelKey] = "abc";
    object2[JsonModel::itemsKey] = QJsonArray();
    EXPECT_TRUE(converter.is_model(object2));
}

//! Creation of json object: empty model.

TEST_F(TestJsonModel, emptyModel)
{
    JsonModel converter;
    SessionModel model("TestModel");

    QJsonObject object;
    converter.to_json(model, object);

    EXPECT_EQ(object[JsonModel::modelKey], "TestModel");
    EXPECT_EQ(object[JsonModel::itemsKey].toArray().size(), 0);

}

//! Checks creation of json object: single item in a model.

TEST_F(TestJsonModel, singleItemInModel)
{
    JsonModel converter;
    SessionModel model("TestModel");

    model.insertNewItem("abc", nullptr, -1);

    QJsonObject object;
    converter.to_json(model, object);

    EXPECT_EQ(object[JsonModel::modelKey], QString::fromStdString(model.modelType()));
    EXPECT_EQ(object[JsonModel::itemsKey].toArray().size(), 1);
}

//! Checks creation of json object: parent and child in a model.

TEST_F(TestJsonModel, parentAndChildInModel)
{
    JsonModel converter;
    SessionModel model("TestModel");

    auto parent = model.insertNewItem("MultiLayer");
    auto child = model.insertNewItem("Layer", parent);

    QJsonObject object;
    converter.to_json(model, object);

    EXPECT_EQ(object[JsonModel::modelKey], QString::fromStdString(model.modelType()));
    EXPECT_EQ(object[JsonModel::itemsKey].toArray().size(), 1);

    // saving to file
    auto fileName = TestUtils::TestFileName(test_dir, "model.json");
    TestUtils::SaveJson(object, fileName);
}
