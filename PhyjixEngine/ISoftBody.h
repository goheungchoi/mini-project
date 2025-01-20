#pragma once

class Vertex;
class ISoftBody
{
public:
	virtual ~ISoftBody() = 0;
    virtual void Initialize(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void SetMaterial(float youngModulus, float poissonRatio, float damping) = 0;
    virtual void ApplyForce(const DirectX::SimpleMath::Vector3& force, const DirectX::SimpleMath::Vector3& position) = 0;
    virtual std::vector<DirectX::SimpleMath::Vector4> GetUpdatedVertices() const = 0;


};

