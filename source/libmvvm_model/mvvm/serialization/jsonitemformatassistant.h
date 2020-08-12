// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#ifndef MVVM_SERIALIZATION_JSONITEMFORMATASSISTANT_H
#define MVVM_SERIALIZATION_JSONITEMFORMATASSISTANT_H

#include <QString>
#include <memory>
#include <mvvm/serialization/jsonitemconverterinterface.h>

namespace ModelView
{

class ItemFactoryInterface;

//! Utility class which carries a set of methods to determine, whether given JSON object can
//! represent an item. It is made a class (and not a set of free functions) to allow different
//! formats in the future.

class MVVM_MODEL_EXPORT JsonItemFormatAssistant
{
public:
    static inline const QString modelKey = "model";
    static inline const QString itemDataKey = "itemData";
    static inline const QString itemTagsKey = "itemTags";
    static inline const QString defaultTagKey = "defaultTag";
    static inline const QString containerKey = "containers";
    static inline const QString tagInfoKey = "tagInfo";
    static inline const QString itemsKey = "items";

    bool isSessionItem(const QJsonObject& json) const;
    bool isSessionItemTags(const QJsonObject& json) const;
    bool isSessionItemContainer(const QJsonObject& json) const;
};

} // namespace ModelView

#endif // MVVM_SERIALIZATION_JSONITEMFORMATASSISTANT_H