// ************************************************************************** //
//
//  Prototype of mini MVVM framework for bornagainproject.org
//
//! @homepage  http://www.bornagainproject.org
//! @license   GNU General Public License v3 or higher
//
// ************************************************************************** //

#ifndef TOYITEMS_H
#define TOYITEMS_H

#include "sessionitem.h"
#include "sessionmodel.h"
#include "itemfactoryinterface.h"
#include <string>

//! Collection of toy items and models for testing purposes.

namespace ToyItems
{

class MultiLayer : public ModelView::SessionItem
{
public:
    static const std::string T_LAYERS;
    MultiLayer();
};

class Layer : public ModelView::SessionItem
{
public:
    Layer();
};

} // ToyItems

#endif // TOYITEMS_H
