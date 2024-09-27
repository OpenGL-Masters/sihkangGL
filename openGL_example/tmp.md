<details><summary> # [W12] </summary>

# Blinn-Phong Shading
- limit of phong lighting model
: If Specular shininess vaule is small, the highlight cut off.(suddenly darked. the boundary of highlight is too obvious)

- When the specular is calculated, view and reflection vector's angle is bigger than 90 degrees.
-> dot product result < 0 ... cutoff 

Blinn의 제안한 방식
view와 light를 이등분하는 벡터 (halfway)와 normal 벡터 간의 사잇각으로 계산해보자
view가 reflection과 일치할때 halfway 벡터는 normal 벡터와 일치하게 된다

## Blinn's way
Using the halfway vector divide half view-light vector's angle.

Phong: ReflectVec -> Blinn: halfwayVec 

Use `vec3 halfDir = normalize(lightDir + viewDir)` when calculate spec value.

# Shadow
Let's make shadow by shadow map.

Idea: Rendering in view of light. If light can see, the pixel has no shadow. But cannot see, the pixel is covered by other object. (shadow)

-> At the position of light source, we will render the object only in depth map(shadow map) by depth test.
then, we check the pixel in position of the camera has been lighted or not by shadow map.

가장 먼저, ShadowMap texture 에 깊이 값 넣어주기. 
빛의 위치에서 화면을 렌더링하여 깊이 값에 대해서만 계산을 진행함(가장 단순한 형태의 쉐이더 사용하여 depth buffer만 채운다)

그렇게 그려진 depth buffer를 프레임버퍼에 바인딩해준 뒤 카메라에서의 렌더링을 진행.
이 때, 각 픽셀의 위치에서 빛의 관점으로 해당 픽셀이 어느 위치에 있는지 계산합니다. 
이를 위해 카메라 좌표계에서 해당 픽셀의 좌표를 빛의 좌표계로 변환한 후, 그 픽셀의 깊이값을 다시 계산합니다.

빛의 위치에서 렌더링한 shadow map에 저장된 깊이값과, 카메라에서 계산된 픽셀의 빛 좌표계 상의 깊이값을 비교합니다.
shadow map의 깊이값 < 카메라에서 계산된 깊이값일 때 해당 픽셀은 그림자로 처리되며, 빛이 물체에 가려진 상태를 나타냅니다.

## shadow acne
깊이값 비교 과정에서 발생하는 정밀도 문제로 발생
-> bias를 더해주는 방식으로 해결

다만 light direction과 surface normal 간의 각도가 크면 acne가 유지되기도 하는데,
이를 대비하여 light direction,, surface normal간의 각도에 따라 bias를 비례하게 만들자.
`float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);`

하지만 bias 값이 너무 크면 **peter panning** 현상이 발생하기도 함.

## PCF
- Percentage Closer Filtering
investigate several depth value from shadow map.
Then get average of shadow value investigated dep value.

## shadow for point light
- Omni-directional shadow map
: make cubmap of Shadow (six faces)

# Normal mapping
Using normal vector texture map for diversity of pixel's normal.
It gives more reality on rendering.

Normal map: the normal xyz valus stored in pixel's rgb value(almost blue)

Use pixelNorm by normal map like texture.

기본적으로 노말맵이라는 이미지를 텍스쳐로 받아와서 사용하는 방식으로 노말벡터를 세팅해줄 수 있음.
하지만 텍스쳐를 그대로 가져와서 사용하게 될 때, 버텍스 위치가 변형되었을 시 월드공간 상에서 노말벡터와 버텍스 간 불일치가 발생할 수 있음.
이를 위해 노말 맵 상의 노말벡터를 안전하게 월드공간으로 변환시켜줄 탄젠트 스페이스를 구하여 그 변환의 inverse-transpose를 노말벡터에 곱해 사용할 것.

따라서, 모든 버텍스에 대한 탄젠트 벡터를 구하고 이를 토대로 TBN 변환을 구해낸다.


</details>