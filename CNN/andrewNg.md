# Method
## Convolution
### Hyper Parameter
* filter size
* filter number
* stride
* padding
* width/height
* channel
    * channel size of this layer based on filter number, rather than the channel size of previous channel

### why
* parameter sharing
* sparcity of connection 

=> robust to translation invariance

=> robust to overfitting => need less training data
### Goal
extract lines in the pictures

the cell can be trained as parameter of deep learning

### 1*1 Convolution
shrink the size of parameter

## Pooling
### HyPer Parameter
* filter size
* stride
* width/height
* channel

### Pooling Method
* Max
* Avg
* Min

### Goal
extract local feature (or concentrate)

no parameter to learn

## Fully Connected

## Layer
* Convolution (+ pooling)
* Full Connected

# Residual Network
* use normal neural network as main path
    $$z^{[l+1]} = w^{[l+1]}a^{[l]} + b^{[l+1]},a^{[l+1]} = g(z^{[l+1]})$$
* use previous layer as short cut (or say skip connection)
$$a^{[l+2]} = g(z^{[l+2]} + a^{[l]}) $$

## Why RN can gets deeper without hurt the performance on training set
since shortcut provides a obvious way to make the output identical to previous layer (where weight and bias all equals to 0 when you apply L2 regularation), it will at least as good as previous layer.

# MobileNet
to reduce computation cost on mobile device
## Depthwise Convolution
makes each channel maps to one filter, rather than maps to all filters
$$previous\ channel\ size = filter\ size = new\ channel\ size $$
## Pointwise Convolution
use 1*1 Depthwise convolution, but use filter sizes to control new channel size

# Transfer learning
freeze network parameter, change the softmax and output layer and train with few data to fit new parameter.

# Object Detection
## Localize
output of the model becomes 
$$
\begin{bmatrix}
isDetected \\
Position \\
Class
\end{bmatrix}
$$
## Landmark Detection
output multiple $Position$ information
### Usage
* face detection
* pose detection 

## Sliding Windows
convolution will naturally acheive sliding windows

## IoU
calculate the satifactory of correctness of boxes detected

## Anchor box 
use multiple predifine shapes as output, to help recognize objects  in different shapes

## Region CNN
use semetic  segmentation to decide which blocks to search, in order to ignore some rigion that obviously not include objects

### Unet 
use a U shape transform to mark different kinds of objects in different label
![unet structure](https://i.stack.imgur.com/bB2tL.jpg)

#### transpose convolution
makes the output in larger dimension

# face identical
## face verification
use similarity function to find a match
$$distance(img1, img2) \leq \tau$$
### siamese network
img -> 128 dimension output

### Object function
use $\alpha$ as a margin, $a$ as the anchor image, $p$ as a positive image and $n$ as a negative image.
$$ d(a,p) + \alpha \leq d(a,n)$$
when choosing a,n,p to train, we prefer that $d(a,p) \approx  d(a,n)$ 