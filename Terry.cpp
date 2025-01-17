﻿#include "Terry.h"
#include "Image.h"
#include "KeyManager.h"
#include "Andy.h"

void Terry::Init()
{
	img = new Image;
	img->Init("Image/Terry/Terry_walk.bmp", 2450, 350, 7, 1, true, RGB(255, 0, 255));
	frameX = frameY = 0;
	elapsedCount = 0;
	pos.x = (WIN_SIZE_X / 5) * 4;
	pos.y = (WIN_SIZE_Y / 6) * 4;
	moveSpeed = 10.0f;
	HP = 100;
	attackValue = 10;
	isHit = false;
	isAlive = true;
	hpimg = new Image;
	hpimg->Init("Image/Terry/Terry_HP_full.bmp", 360, 40, 1, 1, true, NULL);
	pos2.x = (WIN_SIZE_X / 5) * 4;
	pos2.y = (WIN_SIZE_Y / 6);

	countimg = new Image;
	countimg->Init("Image/Time_Count.bmp", 120, 80, 1, 1, true, RGB(255, 0, 255));

	koimg = new Image;
	koimg->Init("Image/Terry/KO_Terry.bmp", 300, 100, 1, 1, true, RGB(255, 0, 255));
	pos3.x = (WIN_SIZE_X / 2);
	pos3.y = (WIN_SIZE_Y / 2);

	hp2img = new Image;
	hp2img->Init("Image/Terry/Terry_HP_red.bmp", 360, 40, 1, 1, true, NULL);

	enemy = new Andy;
	enemyPos.x = 0;
	enemyPos.y = 0;
	charX = img->GetImageInfo()->frameWidth - 280; // 80
}

void Terry::Update()
{
	if (isAlive == false)
		return;
	if (enemyCurrAtk && (pos.x - enemyPos.x < 110) && !isHit)
	{
		isHit = true;
		frameX = 0;
	}
	if (isHit == true)
	{
		Damaged();
	}
	else if (!isHit)
	{
		if (currAtk == false)
		{
			if (KeyManager::GetSingleton()->IsStayKeyDown(VK_NUMPAD6))
			{
				img->Release();
				img = new Image;
				img->Init("Image/Terry/Terry_walk.bmp", 2450, 350, 7, 1, true, RGB(255, 0, 255));
				elapsedCount++;
				if (elapsedCount >= 6)
				{
					frameX++;

					if (pos.x + (charX / 2) < WIN_SIZE_X)
					{
						pos.x += moveSpeed;
					}

					if (frameX >= 7)
					{
						frameX = 0;
					}
					elapsedCount = 0;
				}
			}
			else if (KeyManager::GetSingleton()->IsStayKeyDown(VK_NUMPAD4))
			{
				img->Release();
				img = new Image;
				img->Init("Image/Terry/Terry_walk.bmp", 2450, 350, 7, 1, true, RGB(255, 0, 255));
				elapsedCount++;
				if (elapsedCount >= 6)
				{
					frameX--;

					if (pos.x - (charX / 2) > enemyPos.x + (charX / 2))
					{
						pos.x -= moveSpeed;
					}

					if (frameX < 0)
					{
						frameX = 6;
					}
					elapsedCount = 0;
				}
			}
			else
			{
				if (HP && isAlive)
				{
					AutoMove();
				}
			}

			// ¾à¹ß
			if (KeyManager::GetSingleton()->IsOnceKeyDown('K'))
			{
				frameX = 0;
				isAtk[AttackType::SF] = true;
				currAtk = true;
			}
			// ¾à¼Õ
			if (KeyManager::GetSingleton()->IsOnceKeyDown('I'))
			{
				frameX = 0;
				isAtk[AttackType::SH] = true;
				currAtk = true;
			}
			// °­¹ß
			if (KeyManager::GetSingleton()->IsOnceKeyDown('L'))
			{
				frameX = 0;
				isAtk[AttackType::BF] = true;
				currAtk = true;
			}

			// °­¼Õ
			if (KeyManager::GetSingleton()->IsOnceKeyDown('O'))
			{
				frameX = 0;
				isAtk[AttackType::BH] = true;
				currAtk = true;
			}
		}

		else
		{
			if (isAtk[AttackType::SH])
			{
				Attack(SH);
			}
			else if (isAtk[AttackType::SF])
			{
				Attack(SF);
			}
			else if (isAtk[AttackType::BH])
			{
				Attack(BH);
			}
			else if (isAtk[AttackType::BF])
			{
				Attack(BF);
			}
		}
	}
}

void Terry::Render(HDC hdc)
{
	float temp = (hp2img->GetImageInfo()->width / 100.0f) * (100.f - HP);

	if (img)
	{
		img->Render(hdc, pos.x, pos.y, frameX, frameY);
	}
	if (hpimg)
	{
		hpimg->Render(hdc, pos2.x, pos2.y);
	}
	if (hp2img)
	{
		hp2img->Render(hdc, (pos2.x + (float)(hpimg->GetImageInfo()->width)) - temp
			, pos2.y);
	}
	if (countimg)
	{
		countimg->Render(hdc, pos2.x / 1.6, pos2.y);
	}
	if (koimg)
	{
		if (HP == 0)
		{
			koimg->Render(hdc, pos3.x, pos3.y);
		}
	}
}

void Terry::Release()
{
	if (img)
	{
		delete img;
		img = nullptr;
	}
	if (hpimg)
	{
		delete hpimg;
		hpimg = nullptr;
	}
	if (hp2img)
	{
		delete hp2img;
		hp2img = nullptr;
	}
	if (countimg)
	{
		delete countimg;
		countimg = nullptr;
	}
	if (koimg)
	{
		delete koimg;
		koimg = nullptr;
	}
}

void Terry::AutoMove()
{
	img->Release();
	img = new Image;
	img->Init("Image/Terry/Terry_breath.bmp", 1750, 350, 5, 1, true, RGB(255, 0, 255));
	elapsedCount++;
	if (elapsedCount >= 8)
	{
		if (frameX <= 0)
		{
			frameDir = MoveDir::Right;
		}
		if (frameX >= 4)
		{
			frameDir = MoveDir::Left;
		}
		switch (frameDir)
		{
		case MoveDir::Right:
			frameX++;
			break;
		case MoveDir::Left:
			frameX--;
			break;
		}
		elapsedCount = 0;
	}
}

void Terry::Attack(AttackType type)
{
	string imgName;
	switch (type)
	{
	case SH:
		img->Release();
		img = new Image;
		img->Init("Image/Terry/Terry_weakpunch.bmp", 2450, 350, 7, 1, true, RGB(255, 0, 255));
		elapsedCount++;
		if (elapsedCount >= 3)
		{
			if (frameX == 6)
			{
				for (int i = 0; i < 4; i++)
				{
					if (isAtk[i])
						isAtk[i] = false;
				}
				currAtk = false;
				frameX = 0;
			}
			frameX++;

			elapsedCount = 0;
		}
		break;
	case BH:
		img->Release();
		img = new Image;
		img->Init("Image/Terry/Terry_strongpunch.bmp", 4200, 350, 12, 1, true, RGB(255, 0, 255));

		elapsedCount++;
		if (elapsedCount >= 2)
		{
			if (frameX == 11)
			{
				for (int i = 0; i < 4; i++)
				{
					if (isAtk[i])
						isAtk[i] = false;
				}
				currAtk = false;
				frameX = 0;
			}
			frameX++;

			elapsedCount = 0;
		}
		break;
	case SF:
		img->Release();
		img = new Image;
		img->Init("Image/Terry/Terry_weakkick.bmp", 2450, 350, 7, 1, true, RGB(255, 0, 255));

		elapsedCount++;
		if (elapsedCount >= 4)
		{
			if (frameX == 6)
			{
				for (int i = 0; i < 4; i++)
				{
					if (isAtk[i])
						isAtk[i] = false;
				}
				currAtk = false;
				frameX = 0;
			}
			frameX++;

			elapsedCount = 0;
		}
		break;
	case BF:
		img->Release();
		img = new Image;
		img->Init("Image/Terry/Terry_strongkick.bmp", 2800, 350, 8, 1, true, RGB(255, 0, 255));

		elapsedCount++;
		if (elapsedCount >= 3)
		{
			if (frameX == 7)
			{
				for (int i = 0; i < 4; i++)
				{
					if (isAtk[i])
						isAtk[i] = false;
				}
				currAtk = false;
				frameX = 0;
			}
			frameX++;

			elapsedCount = 0;
		}
		break;
	default:
		break;
	}
}

void Terry::Damaged()
{
	static bool getDamage = false;

	img->Release();
	img = new Image;
	img->Init("Image/Terry/Terry_damaged.bmp", 2100, 350, 6, 1, true, RGB(255, 0, 255));
	elapsedCount++;
	if (HP && isAlive && HP > 0)
    {
        if (elapsedCount >= 15)
        {
            frameX++;
            if (frameX >= 2)
            {
                frameX = 1;
                isHit = false;
                getDamage = false;
                return;
            }
            if (getDamage == false)
            {
                getDamage = true;
                HP -= attackValue;
            }
            elapsedCount = 0;
            return;
        }
    }
	else if (HP <= 0)
	{
		if (elapsedCount >= 30)
		{
			frameX++;
			pos.x += 70;
			elapsedCount = 0;
			if (frameX >= 5)
			{
				isHit = false;
				frameX = 5;
				isAlive = false;
				return;
			}
			return;
		}
	}
}