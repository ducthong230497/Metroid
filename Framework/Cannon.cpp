#include "Cannon.h"
#define CANNOTFIRERATE 1.5
#define MAXBULLETLIVETIME 1
#define SPEED 500
Cannon::Cannon()
{
}

Cannon::~Cannon()
{
}

void Cannon::SetScene(Scene * s)
{
	scene = s;
}

void Cannon::Init(Texture * texture, Cannon::Type type, int randomIndex, int x, int y)
{
	isDestroyed = true;
	lastShot = 0;

	TexturePacker p = TexturePacker(texture, "Resources/bosses_packer.xml");

	cannonAnimation.AddRegion(p.GetRegion("cannon"));
	cannonAnimation.SetIndex(randomIndex);
	bulletAnimation.AddRegion(p.GetRegion("cannonbullet"));
	bulletAnimation.SetIndex(randomIndex);

	SetRegion(*cannonAnimation.GetKeyAnimation());
	SetSize(25, 25);
	SetPosition(x, y);

	//setup body
	//bodyDef.bodyType = Body::BodyType::Static;
	_CategoryMask = CANNON;
	_BitMask = PLAYER;


	//setup bullet
	//BodyDef bulletDef;
	//bulletDef.bodyType = Body::BodyType::Static;
	//bulletDef.position.Set(x, y);
	//bulletDef.size.Set(12, 12);
	//bulletDef.isSensor = true;
	//cannonBullet->body = world->CreateBody(bulletDef);
	cannonBullet = new CannonBullet();
	cannonBullet->cannon = this;
	cannonBullet->_CategoryMask = CANNON_BULLET;
	cannonBullet->_BitMask = PLAYER | PLATFORM;
	//cannonBullet->body->PutExtra(this);

	cannonBullet->SetRegion(*bulletAnimation.GetKeyAnimation());
	cannonBullet->SetSize(12, 12);
	cannonBullet->SetPosition(x, y);

	switch (type)
	{
	case Cannon::Right:
		SetRotation(-180);
		cannonBullet->SetRotation(-180);
		break;
	case Cannon::Top:
		SetRotation(-90);
		cannonBullet->SetRotation(-90);
		break;
	default:
		break;
	}


	//effect
	Animation explosionAnimation;
	explosionAnimation.AddRegion(p.GetRegion("cannonbulletexplosion"));
	bulletExplosionEffect = Effect(explosionAnimation);
	bulletExplosionEffect.SetLiveTime(0.3);
	bulletExplosionEffect.SetSize(15, 15);

}

void Cannon::Render(SpriteBatch * batch)
{
	if (!isDestroyed)
	{
		batch->Draw(cannonBullet);
	}

	batch->Draw(*this);

	bulletExplosionEffect.Render(batch);
}

void Cannon::Update(float dt)
{
	float currentTime = GetTickCount() / 1000.0f;
	if (currentTime > CANNOTFIRERATE + lastShot) //don't shoot more if we have just shooted 
	{
		lastShot = currentTime;
		SetRegion(*cannonAnimation.Next(dt));
		cannonBullet->SetRegion(*bulletAnimation.Next(dt));

		if (isDestroyed) //shoot if bullet is destroyed
		{
			isDestroyed = false;

			int angleOffset;
			if (cannonAnimation.GetIndex() >= 5)
			{
				angleOffset = 8 - cannonAnimation.GetIndex();
			}
			else
			{
				angleOffset = cannonAnimation.GetIndex();
			}

			cannonBullet->setVelocity(-SPEED * sin(45 * angleOffset* Pi / 180 + this->GetRotation()*Pi / 180),
				-SPEED * cos(45 * angleOffset* Pi / 180 + this->GetRotation()*Pi / 180));

			cannonBullet->SetPosition(_Position.x, _Position.y);

		}

	}

	if (isDestroyed == false)
	{
		stateTime += dt;
		if (stateTime > MAXBULLETLIVETIME)
		{
			stateTime = -1;
			isDestroyed = true;
			Trace::Log("Reset bullet");
		}

	}
	else
	{
		stateTime = -1;
	}

	bulletExplosionEffect.Update(dt);
}

void CannonBullet::OnHitGround()
{
	((Cannon*)cannon)->isDestroyed = true;
	((Cannon*)cannon)->bulletExplosionEffect.SetPosition(_Position.x, _Position.y);
	((Cannon*)cannon)->bulletExplosionEffect.Play();
	SetPosition(cannon->getPosition().x, cannon->getPosition().y);
}
