#pragma once
#include "Types.h"

ref class RatingMethod abstract{
public:
	RatingMethod(Controller^ c);
	virtual void Init(SliderTest::GeneralSettings^ gs)=0;
	virtual int getNextItem(void)=0;
	virtual int getsetNextItem()=0;
	virtual bool wasLastTurn(void)=0;
protected:
	Controller^ controller;
};

ref class RatingMethodStandard : public RatingMethod
{
public:
	RatingMethodStandard(Controller^ c);
	virtual void Init(SliderTest::GeneralSettings^) override;
	virtual int getNextItem(void) override;
	virtual int getsetNextItem() override;
	virtual bool wasLastTurn() override {return(false);};

private:
};
ref class RatingMethodConstant : public RatingMethod
{
public:
	RatingMethodConstant(Controller^ c);
	virtual void Init(SliderTest::GeneralSettings^) override;
	virtual int getNextItem(void) override;
	virtual int getsetNextItem() override;
	virtual bool wasLastTurn() override {return(false);};

private:
	int maxnumtrials;
	int nextpos;
};

ref class RatingMethodStaircase : public RatingMethod
{
public:
	RatingMethodStaircase(Controller^ c);
	virtual void Init(SliderTest::GeneralSettings^) override;
	virtual int getNextItem(void) override;
	virtual int getsetNextItem() override;
	virtual bool wasLastTurn() override;

private:
	// Staircase variables
	//int direction;
	int maxnumturns;
	int upAnswer;
	int downAnswer;
	int upTarget;
	int downTarget;
	int maxexceeded;
	int minexceeded;
	int numturns;
};
ref class RatingMethodDouble : public RatingMethod
{
public:
	RatingMethodDouble(Controller^ c);
	virtual void Init(SliderTest::GeneralSettings^) override;
	virtual int getNextItem(void) override;
	virtual int getsetNextItem() override;
	virtual bool wasLastTurn() override;

private:
	int nextplaylist;	// can be 0 or 1
	int currentplaylist;	// can be 0 or 1
	array<int>^ upAnswer;	// 0 for first playlistset 
	array<int>^ downAnswer;	// 1 for second
	array<int>^ numturns;	// number of turnarounds in answers
	int maxnumturns;
	int upTarget;
	int downTarget;
	int maxexceeded;
	int minexceeded;
};

ref class RatingMethodSAMVIQ : public RatingMethod
{
public:
	RatingMethodSAMVIQ(Controller^ c);
	virtual void Init(SliderTest::GeneralSettings^) override;
	virtual int getNextItem(void) override;
	virtual int getsetNextItem() override;
	virtual bool wasLastTurn() override {return(false);};

};

