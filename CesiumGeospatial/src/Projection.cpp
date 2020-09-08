#include "CesiumGeospatial/Projection.h"
#include <cmath>

namespace CesiumGeospatial {

    CesiumGeometry::Rectangle projectRectangleSimple(const Projection& projection, const GlobeRectangle& rectangle) {
        struct Operation {
            const GlobeRectangle& rectangle;

            CesiumGeometry::Rectangle operator()(const WebMercatorProjection& webMercator) {
                return webMercator.project(rectangle);
            }
        };

        return std::visit(Operation { rectangle }, projection);
    }

    GlobeRectangle unprojectRectangleSimple(const Projection& projection, const CesiumGeometry::Rectangle& rectangle) {
        struct Operation {
            const CesiumGeometry::Rectangle& rectangle;

            GlobeRectangle operator()(const WebMercatorProjection& webMercator) {
                return webMercator.unproject(rectangle);
            }
        };

        return std::visit(Operation { rectangle }, projection);
    }

    double computeApproximateConversionFactorToMetersNearPosition(const Projection& projection, const glm::dvec2& position) {
        struct Operation {
            const glm::dvec2& position;

            double operator()(const WebMercatorProjection& webMercator) {
                // TODO: is there a better estimate?
                return std::cos(webMercator.unproject(position).latitude);
            }
        };

        return std::visit(Operation { position }, projection);
    }
}
