# HDR - Hight Dynamic Range
기존의 픽셀의 색상값 처리는 1.0을 넘어가는 경우 clamp 처리를 하여 색상값이 0 ~ 1 사이 범위에 존재하게 헀음.
하지만 실제로는 모니터에서 처리 불가능한 1.0을 넘는 값들이 들어오게 되는 경우들이 있는데 이들에 대한 자연스러운 처리가 안된다는 단점이 존재.

이에, 일시적으로 1.0 이상의 값도 받아둔 뒤, 색상값을 0 ~ 1 사이 범위로 조정하는 과정을 거치게 한다.

## tone mapping
우선 조금 더 자연스러운 rgb처리를 위해서, rgb가 가지는 값 자체를 clamp하지 않고 받아들여야한다.
-> 프레임버퍼 생성 시 기존 `GL_RGBA` 형식으로 8비트 * 4 만큼을 담았던 것에서 `GL_RGBA16F`, `GL_RGBA32F`등으로 저장공간을 넓히기.
1.0 이상의 데이터를 담기위해 필요함.

- Reinhard tone mapping: 담겨진 색상값을 0 ~ 1 사이 범위로 매핑
`vec3 mapped = hdrColor / (hdrColor + vec3(1.0))`

- Exposure tone mapping: 노출값을 파라미터로 사용하여 그에 따라 밝기를 조절.
`vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure)`

# Bloom
glow effect 후처리 방식.
기본적으로 렌더링한 화면과 밝기가 1을 넘는 픽셀을 랜더링한 뒤 기본 렌더링 화면에 합산하여 tone mapping 효과를 넣는 식으로 진행.
이 때 적용하는 블룸효과로는 `Gaussian blur`를 주로 적용

# Deferred shading
현재까지는 Forward shading 방식으로 그림을 그려왔음.
light source, object가 많아질수록 성능 저하가 발생.

Forward Shading에서는 모든 광원에 대해 각 물체가 별도로 조명 연산을 수행해야 하므로, 광원이 늘어날수록 연산량이 증가함.
또한 실제로 렌더링되는 픽셀 중에서 일부는 여러 번 그려질 필요가 없는데도, Forward Shading은 투명도나 그림자 같은 요소에 대해 불필요하게 모든 광원과 물체의 계산을 반복적으로 수행합니다.

이에 조금 더 효율적으로 렌더링하기 위해 `Deferred shading` 방식 도입.
light 관련된 모든 연산을 기존 순서에서 미루어 마지막 단계에서 수행함.

이를 위해서 우선적으로 `G-buffer`라는 light 계산에 필요한 position, texture data 등을 담아 렌더링할 버퍼를 만할
이렇게 G-buffer에 담겨져있는 데이터를 기반으로 마지막에 lighting 계산을 진행하게 됨.

G-Buffer는 라이팅 계산을 위한 텍스쳐 뭉치라고 생각하면 됨. (3D world space position, Albedo (= diffuse color, 기본 배색), 3D normal, Specular intensity, Light source position & color, View position vector)


