#ifndef MATERIALX_HWSOURCECODENODE_H
#define MATERIALX_HWSOURCECODENODE_H

#include <MaterialXGenShader/Nodes/SourceCodeNode.h>

namespace MaterialX
{

/// Extending the SourceCodeNode with requirements for HW.
class HwSourceCodeNode : public SourceCodeNode
{
public:
    static ShaderNodeImplPtr create();

    void emitFunctionCall(const ShaderNode& node, GenContext& context, ShaderStage& stage) const override;
};

} // namespace MaterialX

#endif