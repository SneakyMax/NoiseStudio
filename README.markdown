#Nodes

###BlankGrid

Creates an attribute grid, which is usually used to make an image. A grid has no inherent sense of a 
"pixel", it just says that it is a multidimensional array (flattened) of *something*.

#####Inputs

*   **Width** - uniform scalar unsigned int. Optional. If not set, will take the value of the **Width** property.
*   **Height** - uniform scalar unsigned int. Optional. If not set, will take the value of the **Height** property.

#####Outputs

*   **Grid** - attribute (undefined). Does not actually output values, just sets the graph to a grid of values. 
               e.g., a Grid of 10x10 will output an attribute array of 100.

#####Properties

*   **Width** - uniform scalar unsigned int. Sets the width of the grid. If the **Width** input is connected, this value is overridden. Defaults to 512. 
*   **Height** - uniform scalar unsigned int. Sets the height of the grid. If the **Height** input is connected, this value is overridden. Defaults to 512.


###ConstantValue

Outputs a single uniform value.

#####Inputs

*   **None**

#####Outputs

*   **Value** - The value which is set on the constant value. Will be the same type and scalar/vector size as the value. Call set_value or set_value_single to set the value.

#####Properties

*   **None**


###Math

Executes math functions on uniforms and attributes.

#####Inputs

*   **A** - any type. The first value for the operation. Must be the same type as B.
*   **B** - any type. The second value for the operation. Only available if the math node is set to a *unary* operation. Must be the same type as A.

#####Outputs

*   **Output** - If **A** or **B** are an attribute, is an attribute. Otherwise is a uniform. Is the same type as the inputs.

#####Properties

*   **Operation** - int, but prever using the MathOperation enum. Sets the math operation to be performed. Defaults to add.


###TypeConversion

Converts from one type to another.

#####Inputs

*   **Input** - any type. Attribute or uniform. The value(s) to be converted.
*   **Output** - the output type selected in the **Target Type** property.

#####Outputs

*   **Target Type** - int, but prefer using TypeConversionTargetType enum. Sets the type to be converted to.

#####Properties

*   **None**


##PerlinNoise

Generates perlin noise.

#####Inputs

*   **Seed** - uniform scalar long. Sets the seed to be used for the random number generator. If the same seed is passed into two (same size) perlin noise nodes. The outputs will be the same.
*   **Points** - attribute float 2/3/4/5/6. The value of the perlin noise function will be calculated for each value in this array.

#####Outputs

*   **Output** - attribute float 1. The value of the noise function.

#####Properties

*   **None**


##Mappings/PixelMapping

For a grid, maps each point to an x/y position for the grid.

#####Inputs

*   **Grid** - attribute, any type. The grid to calculate a pixel mapping for.

#####Outputs

*   **Mapped** - attribute, int2. The mapped grid to pixels. Each attribute point will contain an int2 that contains the x/y position of that point.

#####Properties

*   **None**