#ifndef MATERIALX_OslValidator_H
#define MATERIALX_OslValidator_H

#include <MaterialXRender/ShaderValidators/ShaderValidator.h>
#include <MaterialXRender/ShaderValidators/ExceptionShaderValidationError.h>
#include <MaterialXRender/Handlers/ImageHandler.h>
#include <vector>
#include <string>
#include <map>

namespace MaterialX
{
// Shared pointer to an OslValidator
using OslValidatorPtr = std::shared_ptr<class OslValidator>;

/// @class @OslValidator
/// Helper class to perform validation of OSL source code generated by an OSL code generator.
///
/// The main services provided are:
///     - Source code validation: Use of oslc to compile and test output results
///     - Introspection check: None at this time.
///     - Binding: None at this time.
///     - Render validation: Use of "testshade" to output rendered images. Assumes source compliation was success
///       as it depends on the existence of corresponding .oso files.
///
class OslValidator : public ShaderValidator
{
  public:
    /// Create an OSL validator instance
    static OslValidatorPtr create();

    /// Destructor
    virtual ~OslValidator();

    /// Color closure OSL string
    static std::string OSL_CLOSURE_COLOR_STRING;

    /// @name Setup
    /// @{

    /// Internal initialization required for program validation and rendering.
    /// An exception is thrown on failure.
    /// The exception will contain a list of initialization errors.
    void initialize() override;

    /// @}
    /// @name Validation
    /// @{

    /// Validate creation of an OSL program based on an input shader
    ///
    /// A valid executable and include path must be specified before calling this method.
    /// setOslCompilerExecutable(), and setOslIncludePath(). 
    ///
    /// Additionally setOslOutputFilePath() should be set to allow for output of .osl and .oso
    /// files to the appropriate path location to be used as input for render validation.
    /// 
    /// If render validation is not required, then the same temporary name will be used for
    /// all shaders validated using this method.
    /// @param shader Input shader
    void validateCreation(const ShaderPtr shader) override;

    /// Validate creation of an OSL program based upon a shader string for a given
    /// shader "stage". There is only one shader stage for OSL thus
    /// @param stages List of shader strings. Only first string in list is examined.
    void validateCreation(const std::vector<std::string>& stages) override;

    /// Validate inputs for the compiled OSL program. 
    /// Note: Currently no validation has been implemented.
    void validateInputs() override;

    /// Validate that an appropriate rendered result is produced.
    /// This is done by using either "testshade" or "testrender".
    /// Currently only "testshade" is supported.
    ///
    /// Usage of both executables requires compiled source (.oso) files as input.
    /// A shader output must be set before running this test via the setOslOutputName() method to
    /// ensure that the appropriate .oso files can be located.
    ///
    /// @param orthographicView Render orthographically
    void validateRender(bool orthographicView=true) override;

    /// @}
    /// @name Utilities
    /// @{

    /// Save the current contents a rendering to disk. Note that this method
    /// does not perform any action as validateRender() produces images as part if it's
    /// execution.
    /// @param fileName Name of file to save rendered image to.
    void save(const std::string& fileName) override;
    
    /// @}
    /// @name Compilation settings
    /// @{

    /// Set the OSL executable path string. Note that it is assumed that this
    /// references the location of the oslc executable.
    /// @param executable Full path to OSL compiler executable
    void setOslCompilerExecutable(const std::string executable)
    {
        _oslCompilerExecutable = executable;
    }

    /// Set the OSL include path string. 
    /// @param includePathString Include path(s) for the OSL compiler. This should include the
    /// path to stdosl.h    
    void setOslIncludePath(const std::string includePathString)
    {
        _oslIncludePathString = includePathString;
    }

    /// Set OSL output name, excluding any extension.
    /// During compiler checking an OSL file of the given output name will be used if it
    /// is not empty. If temp then OSL will be written to a temporary file.
    /// @param filePathString Full path name
    void setOslOutputFilePath(const std::string filePathString)
    {
        _oslOutputFilePathString = filePathString;
    }

    /// Set the OSL shader output name. 
    /// This is used during render validation if "testshade" or "testrender" is executed.
    /// For testrender this value is used to replace the %shader_output% token in the
    /// input scene file.
    /// @param outputName Name of shader output
    /// @param outputName The MaterialX type of the output
    /// @param remappedShaderOutput Has color and vector shader output been remapped to to color3
    void setOslShaderOutputNameAndType(const std::string outputName, const std::string outputType,
                                       bool remappedShaderOutput)
    {
        _oslShaderOutputName = outputName;
        _oslShaderOutputType = outputType;
        _remappedShaderOutput = remappedShaderOutput;
    }

    /// Set the OSL shading tester path string. Note that it is assumed that this
    /// references the location of the "testshade" executable.
    /// @param executable Full path to OSL "testshade" executable
    void setOslTestShadeExecutable(const std::string executable)
    {
        _oslTestShadeExecutable = executable;
    }

    /// Set the OSL rendering tester path string. Note that it is assumed that this
    /// references the location of the "testrender" executable.
    /// @param executable Full path to OSL "testrender" executable
    void setOslTestRenderExecutable(const std::string executable)
    {
        _oslTestRenderExecutable = executable;
    }

    /// Set the XML scene file to use for testrender. This is a template file
    /// with the following tokens for replacement:
    ///     - %shader% : which will be replaced with the name of the shader to use
    ///     - %shader_output% : which will be replace with the name of the shader output to use
    /// @param templateFileName Scene file name
    void setOslTestRenderSceneTemplateFile(const std::string templateFileName)
    {
        _oslTestRenderSceneTemplateFile = templateFileName;
    }

    /// Set the name of the shader to be used for the input XML scene file.
    /// The value is used to replace the %shader% token in the file.
    /// @param shaderName Name of shader
    void setOslShaderName(const std::string shaderName)
    {
        _oslShaderName = shaderName;
    }

    /// Set the search path for dependent shaders (.oso files) which are used
    /// when rendering with testrender. 
    /// @param osoPath Path to .oso files.
    void setOslUtilityOSOPath(const std::string osoPath)
    {
        _oslUtilityOSOPath = osoPath;
    }

    /// Used to toggle to either use testrender or testshade during render validation
    /// By default testshade is used.
    /// @param useTestRender Indicate whether to use testrender.
    void useTestRender(bool useTestRender)
    {
        _useTestRender = useTestRender;
    }

    ///
    /// Compile OSL code stored in a file. Will throw an exception if an error occurs.
    /// @param oslFileName Name of OSL file
    void compileOSL(const std::string& oslFileName);

    /// @}

  protected:
    ///
    /// Shade using OSO input file. Will throw an exception if an error occurs.
    /// @param outputPath Path to input .oso file.
    /// @param shaderName Name of OSL shader. A corresponding .oso file is assumed to exist in the output path folder.
    /// @param outputName Name of OSL shader output to use.
    void shadeOSL(const std::string& outputPath, const std::string& shaderName, const std::string& outputName);

    ///
    /// Render using OSO input file. Will throw an exception if an error occurs.
    /// @param outputPath Path to input .oso file.
    /// @param shaderName Name of OSL shader. A corresponding .oso file is assumed to exist in the output path folder.
    /// @param outputName Name of OSL shader output to use.
    void renderOSL(const std::string& outputPath, const std::string& shaderName, const std::string& outputName);

    /// Constructor
    OslValidator();

  private:
    /// "oslc" executable name`
    std::string _oslCompilerExecutable;
    /// OSL include path name
    std::string _oslIncludePathString;
    /// Output file path. File name does not include an extension
    std::string _oslOutputFilePathString;

    /// "testshade" executable name
    std::string _oslTestShadeExecutable;
    /// "testrender" executable name
    std::string _oslTestRenderExecutable;
    /// Template scene XML file used for "testrender"
    std::string _oslTestRenderSceneTemplateFile;
    /// Name of shader. Used for rendering with "testrender"
    std::string _oslShaderName;
    /// Name of output on the shader. Used for rendering with "testshade" and "testrender"
    std::string _oslShaderOutputName;
    /// MaterialX type of the output on the shader. Used for rendering with "testshade" and "testrender"
    std::string _oslShaderOutputType;
    /// Has color and vector output been remapped to 3-channel color
    bool _remappedShaderOutput;
    /// Path for utility shaders (.oso) used when rendering with "testrender"
    std::string _oslUtilityOSOPath;
    /// Use "testshade" or "testender" for render validation
    bool _useTestRender;
};

} // namespace MaterialX
#endif
