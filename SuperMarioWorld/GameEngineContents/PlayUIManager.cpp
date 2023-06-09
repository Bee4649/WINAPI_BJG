#include "PlayUIManager.h"
#include "ContentsEnum.h"
#include <GameEngineCore/GameEngineRenderer.h>

PlayUIManager::PlayUIManager() 
{
}

PlayUIManager::~PlayUIManager() 
{
}

void PlayUIManager::SetLife(int Value)
{
	LifeNumber.SetValue(Value);
}

void PlayUIManager::SetStar(int Value)
{
	StarNumber.SetValue(Value);
}

void PlayUIManager::SetTime(int Value)
{
	TimeNumber.SetValue(Value);
}

void PlayUIManager::SetCoin(int Value)
{
	CoinNumber.SetValue(Value);
}

void PlayUIManager::SetScore(int Value)
{
	ScoreNumber.SetValue(Value);
}

void PlayUIManager::SetValue(int Life, int Star, int CoinNum, int Score)
{
	LifeNumber.SetValue(Life);
	StarNumber.SetValue(Star);
	CoinNumber.SetValue(CoinNum);
	ScoreNumber.SetValue(Score);
}

void PlayUIManager::SetStockItem(ItemType _Item)
{
	switch (_Item)
	{
	case ItemType::SuperMushroom:
		StockImageRender->SetImage("SUPERMUSHROOM.BMP");
		break;
//	case ItemType::FireFlower:
//		StockImageRender->SetImage("FIREFLOWERUI.BMP");
//		break;
	default:
		StockImageRender->SetImage("NONE.BMP");
		break;
	}
}

void PlayUIManager::ClearUIOn(bool _IsStarBonus)
{
	if (true == _IsStarBonus)
	{
		ClearRender->SetImage("ClearBonus.bmp");
		StarBonus.On();
	}
	else
	{
		ClearRender->SetImage("Clear.bmp");
		StarBonus.Off();
	}
	ClearRender->On();
	ClearTimeNumber.On();
	TimeBonus.On();
}

void PlayUIManager::ClearUIOff()
{
	ClearRender->Off();
	ClearTimeNumber.Off();
	TimeBonus.Off();
	StarBonus.Off();
}

void PlayUIManager::SetClearBonus(int _ClearTime, int _TimeBonus, int _StarBonus)
{
	ClearTimeNumber.SetValue(_ClearTime);
	TimeBonus.SetValue(_TimeBonus);
	StarBonus.SetValue(_StarBonus);
}

void PlayUIManager::SetTimeBonus(int _TimeBonus)
{
	TimeBonus.SetValue(_TimeBonus);
}

void PlayUIManager::SetStarBonus(int _StarBonus)
{
	StarBonus.SetValue(_StarBonus);
}


void PlayUIManager::Start()
{
	UIImage = CreateRender("PLAYUI.bmp", RenderOrder::UI);
	UIImage->SetScaleToImage();
	UIImage->EffectCameraOff();
	UIImage->SetPosition({ 512, 128 });

	LifeNumber.SetOwner(this);
	LifeNumber.SetImage("NUMBER.BMP", NumberScale, static_cast<int>(RenderOrder::UI), RGB(255, 0, 255), "NONE.bmp");
	LifeNumber.SetRenderPos({ 180, 110 });
	LifeNumber.SetAlign(Align::Right);
	LifeNumber.SetValue(5);

	StarNumber.SetOwner(this);
	StarNumber.SetImage("NUMBERBIG.BMP", NumberBigScale, static_cast<int>(RenderOrder::UI), RGB(255, 0, 255), "NONE.bmp");
	StarNumber.SetRenderPos({ 430, 100 });
	StarNumber.SetAlign(Align::Right);
	StarNumber.SetValue(0);

	TimeNumber.SetOwner(this);
	TimeNumber.SetImage("NUMBERYELLOW.BMP", NumberScale, static_cast<int>(RenderOrder::UI), RGB(255, 0, 255), "NONE.bmp");
	TimeNumber.SetRenderPos({ 690, 110 });
	TimeNumber.SetAlign(Align::Right);
	TimeNumber.SetValue(300);

	CoinNumber.SetOwner(this);
	CoinNumber.SetImage("NUMBER.BMP", NumberScale, static_cast<int>(RenderOrder::UI), RGB(255, 0, 255), "NONE.bmp");
	CoinNumber.SetRenderPos({ 948, 76 });
	CoinNumber.SetAlign(Align::Right);
	CoinNumber.SetValue(0);

	ScoreNumber.SetOwner(this);
	ScoreNumber.SetImage("NUMBER.BMP", NumberScale, static_cast<int>(RenderOrder::UI), RGB(255, 0, 255), "NONE.bmp");
	ScoreNumber.SetRenderPos({ 948, 110 });
	ScoreNumber.SetAlign(Align::Right);
	ScoreNumber.SetValue(0);


	StockImageRender = CreateRender("NONE.BMP", RenderOrder::UI);
	StockImageRender->EffectCameraOff();
	StockImageRender->SetScale({ 64, 64 });
	StockImageRender->SetPosition({ 512, 92 });


	BlackRender = CreateRender("ALLBLACK.BMP", RenderOrder::Monster);
	BlackRender->SetScaleToImage();
	BlackRender->EffectCameraOff();
	BlackRender->SetPosition(BlackRender->GetScale().half());
	BlackRender->SetAlpha(0);

	ClearRender = CreateRender("ClearBonus.bmp", RenderOrder::UI);
	ClearRender->SetScaleToImage();
	ClearRender->EffectCameraOff();
	ClearRender->SetPosition(BlackRender->GetScale().half());
	ClearRender->Off();

	ClearTimeNumber.SetOwner(this);
	ClearTimeNumber.SetImage("NUMBER.BMP", NumberScale, static_cast<int>(RenderOrder::UI), RGB(255, 0, 255), "NONE.bmp");
	ClearTimeNumber.SetRenderPos({ 400, 430 });
	ClearTimeNumber.SetAlign(Align::Right);
	ClearTimeNumber.SetValue(300);
	ClearTimeNumber.Off();

	TimeBonus.SetOwner(this);
	TimeBonus.SetImage("NUMBER.BMP", NumberScale, static_cast<int>(RenderOrder::UI), RGB(255, 0, 255), "NONE.bmp");
	TimeBonus.SetRenderPos({ 688, 430 });
	TimeBonus.SetAlign(Align::Right);
	TimeBonus.SetValue(15000);
	TimeBonus.Off();

	StarBonus.SetOwner(this);
	StarBonus.SetImage("NUMBERBIG.BMP", NumberBigScale, static_cast<int>(RenderOrder::UI), RGB(255, 0, 255), "NONE.bmp");
	StarBonus.SetRenderPos({ 672, 512 });
	StarBonus.SetAlign(Align::Right);
	StarBonus.SetValue(50);
	StarBonus.Off();

}

void PlayUIManager::Update(float _DeltaTime)
{
}

void PlayUIManager::Render(float _DeltaTime)
{
}
