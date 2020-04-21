// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#include "test_utils.h"
#include "testconfig.h" // this file is auto generated by build system in build directory
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <filesystem>
#include <mvvm/model/sessionmodel.h>
#include <mvvm/serialization/jsonconverterinterfaces.h>
#include <mvvm/serialization/jsonutils.h>
#include <string>

using namespace ModelView;

namespace
{
void SaveDocument(const QJsonDocument& document, const std::string& fileName);
}

void TestUtils::CreateTestDirectory(const std::string& test_sub_dir)
{
    auto path = TestDirectoryPath(test_sub_dir);
    std::filesystem::create_directory(std::filesystem::path(path));
}

std::string TestUtils::TestDirectoryPath(const std::string& test_sub_dir)
{
    auto result = std::filesystem::path(TestConfig::TestOutputDir()) / test_sub_dir;
    return result.string();
}

std::string TestUtils::TestFileName(const std::string& test_sub_dir, const std::string& file_name)
{
    std::filesystem::path result =
        std::filesystem::path(TestDirectoryPath(test_sub_dir)) / file_name;
    return result.string();
}

void TestUtils::SaveJson(const QJsonObject& object, const std::string& fileName)
{
    QJsonDocument document(object);
    SaveDocument(document, fileName);
}

void TestUtils::SaveJson(const QJsonArray& object, const std::string& fileName)
{
    QJsonDocument document(object);
    SaveDocument(document, fileName);
}

QString TestUtils::JsonToString(const QJsonObject& object)
{
    QJsonDocument document(object);
    return QString(document.toJson(QJsonDocument::Compact));
}

QString TestUtils::ModelToJsonString(SessionModel& model)
{
    return QString::fromStdString(JsonUtils::ModelToJsonString(model));
}

QJsonDocument TestUtils::LoadJson(const std::string& fileName)
{
    QFile jsonFile(QString::fromStdString(fileName));

    if (!jsonFile.open(QIODevice::ReadOnly))
        throw std::runtime_error("TestUtils::LoadJson() -> Can't read file");

    return QJsonDocument().fromJson(jsonFile.readAll());
}

namespace
{

void SaveDocument(const QJsonDocument& document, const std::string& fileName)
{
    QFile saveFile(QString::fromStdString(fileName));

    if (!saveFile.open(QIODevice::WriteOnly))
        throw std::runtime_error("TestUtils::SaveDocument() -> Can't save file");

    saveFile.write(document.toJson());
}

} // namespace
