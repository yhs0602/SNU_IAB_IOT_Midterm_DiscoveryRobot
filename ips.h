/*
 * Indoor Positioning System
 * @Author: 2019-13674 양현서(KYHSGeekCode)
 * @Desc: Indoor Positioning System을 구현하기 위한 프로토타입이었으나,
 * https://stackoverflow.com/a/58422150/8614565
 * 위의 내용에 따르면 신호의 본래 세기를 얻어오는 것이 쉽지 않아
 * 보류하였습니다.
 */
class IPS
{
  public:
    IPS();
    ~IPS();
    int SearchAPs();
    bool getMyPosition(float &x, float &y, float &z);
};
