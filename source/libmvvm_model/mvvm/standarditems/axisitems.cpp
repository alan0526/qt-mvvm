// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#include <mvvm/standarditems/axisitems.h>
#include <mvvm/standarditems/plottableitems.h>

namespace
{
const double default_axis_min = 0.0;
const double default_axis_max = 1.0;
} // namespace

using namespace ModelView;

BasicAxisItem::BasicAxisItem(const std::string& model_type) : CompoundItem(model_type) {}

void BasicAxisItem::register_min_max()
{
    addProperty(P_MIN, default_axis_min)->setDisplayName("Min")->setLimits(RealLimits::limitless());
    addProperty(P_MAX, default_axis_max)->setDisplayName("Max")->setLimits(RealLimits::limitless());
}

// --- ViewportAxisItem ------------------------------------------------------

ViewportAxisItem::ViewportAxisItem(const std::string& model_type) : BasicAxisItem(model_type)
{
    addProperty<TextItem>(P_TITLE)->setDisplayName("Title");
    register_min_max();
    addProperty(P_IS_LOG, false)->setDisplayName("log10");
}

//! Returns pair of lower, upper axis range.

std::pair<double, double> ViewportAxisItem::range() const
{
    return std::make_pair(property<double>(P_MIN), property<double>(P_MAX));
}

//! Sets lower, upper range of axis to given values.

void ViewportAxisItem::set_range(double lower, double upper)
{
    setProperty(P_MIN, lower);
    setProperty(P_MAX, upper);
}

bool ViewportAxisItem::is_in_log() const
{
    return property<bool>(P_IS_LOG);
}

// --- BinnedAxisItem ------------------------------------------------------

BinnedAxisItem::BinnedAxisItem(const std::string& model_type) : BasicAxisItem(model_type)
{
    addProperty(P_NBINS, 1)->setDisplayName("Nbins");
    register_min_max();
}

std::pair<double, double> BinnedAxisItem::range() const
{
    return std::make_pair(property<double>(P_MIN), property<double>(P_MAX));
}

int BinnedAxisItem::size() const
{
    return property<int>(P_NBINS);
}

// --- FixedBinAxisItem ------------------------------------------------------

FixedBinAxisItem::FixedBinAxisItem() : BinnedAxisItem(Constants::FixedBinAxisItemType) {}

std::unique_ptr<FixedBinAxisItem> FixedBinAxisItem::create(int nbins, double xmin, double xmax)
{
    auto result = std::make_unique<FixedBinAxisItem>();
    result->setProperty(P_NBINS, nbins);
    result->setProperty(P_MIN, xmin);
    result->setProperty(P_MAX, xmax);
    return result;
}

std::vector<double> FixedBinAxisItem::binCenters() const
{
    std::vector<double> result;
    int nbins = property<int>(P_NBINS);
    double start = property<double>(P_MIN);
    double end = property<double>(P_MAX);
    double step = (end - start) / nbins;

    result.resize(static_cast<size_t>(nbins), 0.0);
    for (size_t i = 0; i < static_cast<size_t>(nbins); ++i)
        result[i] = start + step * (i + 0.5);

    return result;
}

// --- PointwiseAxisItem ------------------------------------------------------

PointwiseAxisItem::PointwiseAxisItem() : BinnedAxisItem(Constants::PointwiseAxisItemType)
{
    // vector of points matching default xmin, xmax
    setData(std::vector<double>{default_axis_min, default_axis_max});
}

std::unique_ptr<PointwiseAxisItem> PointwiseAxisItem::create(const std::vector<double>& data)
{
    auto result = std::make_unique<PointwiseAxisItem>();
    result->setData(data);
    result->setProperty(P_NBINS, static_cast<int>(data.size()));
    result->setProperty(P_MIN, data.front());
    result->setProperty(P_MAX, data.back());

    // Nbins, min, max are defined via factory ::create method and shouldn't be changed after.
    // Flags below prevents their editing in widgets, but doesn't help against direct modification
    result->getItem(P_NBINS)->setEditable(false);
    result->getItem(P_MIN)->setEditable(false);
    result->getItem(P_MAX)->setEditable(false);

    // Now to make nbinx, xmin, xmax consistent with data<> points?

    return result;
}

std::vector<double> PointwiseAxisItem::binCenters() const
{
    return data<std::vector<double>>();
}
