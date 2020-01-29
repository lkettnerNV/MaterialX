//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_RTGENERIC_H
#define MATERIALX_RTGENERIC_H

/// @file
/// TODO: Docs

#include <MaterialXRuntime/RtSchema.h>

namespace MaterialX
{

/// @class RtGeneric
/// Schema for generic/unknown prims.
class RtGeneric : public RtTypedSchema
{
    DECLARE_TYPED_SCHEMA(RtGeneric)

public:
    /// Get the kind for this generic prim,
    /// which gives its custom typename.
    const RtToken& getKind() const;

    /// Set the kind for this generic prim,
    /// giving its custom typename.
    void setKind(const RtToken& kind) const;
};

}

#endif
