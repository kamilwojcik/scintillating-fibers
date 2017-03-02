#ifndef EVENT_H
#define EVENT_H



class Event : public TObject {
  public:
  Event(){};
  ~Event(){};
  
  Int_t qdcTrigger[2];
  Int_t tdcTrigger[2];
  Int_t qdcArray[2][16];
  Int_t tdcArray[2][16];
  Int_t tdcRibbon[2][3][16];
  Int_t qdcRibbon[2][3][16];

  int evntNum;
};



#endif // EVENT_H
