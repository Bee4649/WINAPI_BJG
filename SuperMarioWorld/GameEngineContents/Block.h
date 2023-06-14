#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/GameEngineCollision.h>
#include "ContentsEnum.h"

const float BlockYSize = 50.0f;			// ���� ���� ����
const float BlockXSize = 31.0f;			// ���� ���� ������ �Ÿ�
const float BlockSidePos = 47.0f;		// ���� ���� ������ �Ÿ�
const float BlockOnPos = 63.0f;			// ���� ���� ���� �Ÿ�

class Block : public GameEngineActor
{
public:
	Block();
	~Block();

	Block(const Block& _Other) = delete;
	Block(Block&& _Other) noexcept = delete;
	Block& operator=(const Block& _Other) = delete;
	Block& operator=(Block&& _Other) noexcept = delete;

	virtual void Hit();

	/// ������ ���������� �ı��� ����Ǵ� �Լ�. ������ �ı��ɽ� true, �ı����� �ʴ� �����̸� false�� ����
	virtual bool Damage();
	virtual bool GetIsRoll() { return false; }

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render(float _DeltaTime) override;

	virtual void HitAnimEnd() = 0;

	GameEngineRenderer* AnimationRender = nullptr;
	const float4 RenderScale = { 64, 64 };
	const float4 RenderPos = { 0, -31 };

private:
	const float HitAnimSpeed = 800;
	const float4 CollisionScale = { 70, 70 };
	const float4 CollisionPos = { 0, -35 };
	const float4 HitCollisionScale = { 32, 16 };
	const float4 HitCollisionPos = { 0, -64 };
	const float4 BlockdebritsForce = { 250, -750 };

	const float BlockdebritsLiveTime = 2;
	bool IsHit = false;
	float HitTimer = 0;
	const float HitTime = 0.05f;
	const float HitTime2 = 0.1f;

	GameEngineCollision* Collision = nullptr;
	GameEngineCollision* HitCollision = nullptr;
};