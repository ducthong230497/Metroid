#pragma once
#include "GameObject.h"
#define NOT_COLLIDED 10000.0f
class Collision
{
private:

	// Kết quả của thuật toán kiểm tra va chạm,
	// không có va chạm sẽ trả về 1, 
	float _CollisionRatio;

	// Kết quả của va chạm tác động đến object,
	// dùng đề xác định hướng va chạm của các object (trên dưới trái phải)
	POINT _CollisionDirection;

	// Thời gian còn lại trong 1 frame sau khi va chạm xảy ra 
	// (thời gian của 1 frame > thời điểm va chạm - thời điểm bắt đầu frame)
	float _RemainingTime;

	POINT _CollisionPosition; // Vị trí của vật di chuyển khi va chạm với vật đứng yên

	float target_tempvx, target_tempvy; //velocity của vật a khi xem vật b đang đứng yên(nếu thực sự vật b đang di chuyển)
	float dxentry, dyentry, dxexit, dyexit; //dx entry / exit, dy entry / exit, khoảng d cần cần vật a đụng độ/ thoát ra khỏi vật b đứng yên (delta d của vật a trong khoảng delta_time)
	float deltadx, deltady; //khoảng delta vật a đi được trong khoảng thời gian delta_time
	float rxentry, rxexit, ryentry, ryexit;
	float rentry;  //khoảng thời gian cần để thực sự xảy ra đụng độ
	float rexit; 	//khoảng thời gian cần để vật a thực sự thoát khỏi vật b

	float moveX, moveY;

	bool IsSensorEntered;

private: //private function

	void UpdateTargetPosition(GameObject *GameObject, const POINT &move);

	void Push(GameObject *GameObject);
	void Slide(GameObject *GameObject);
	void Deflect(GameObject *GameObject);

public:
	Collision();
	~Collision();

	RECT GetBroadphaseRect(GameObject *GameObject, float DeltaTime);
	RECT GetRECT(GameObject *GameObject);
	bool Collide(GameObject *targetGameObject, GameObject *otherGameObject, float DeltaTime);
	bool CanMaskCollide(GameObject *targetGameObject, GameObject *otherGameObject);
	
	// Hàm dùng để dự đoán vùng bao phủ của object trong frame kế
	bool IsOverlayingRect(const RECT &rect1, const RECT &rect2);

	void PerformCollision(GameObject *targetGameObject, GameObject *otherGameObject, float DeltaTime, int collisionAction, bool &needMoveX, bool &needMoveY);

	//bool IsPreviousCollding(GameObject *targetGameObject, GameObject *otherGameObject, float DeltaTime);

	//return 1 if bottom or top, return 2 if left or right and return 0 if not previous touching
	int IsPreviousTouching(GameObject *targetGameObject, GameObject *otherGameObject);

	int IsTouching(GameObject *targetGameObject, GameObject *otherGameObject);

	// 2 GameObject có chồng lên nhau hay không
	bool IsOverlaying(GameObject *targetGameObject, GameObject *otherGameObject);

	bool IsPreviousOverlayed(GameObject *targetGameObject, GameObject *otherGameObject);


	void PerformOverlaying(GameObject *targetGameObject, GameObject *otherGameObject, bool &needMoveX, bool &needMoveY);

	//void SetContactListener(WorldContactListener *listener);

	//Reset to use as a new one
	void Reset();


};