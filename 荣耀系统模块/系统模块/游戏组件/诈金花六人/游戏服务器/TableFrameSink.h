#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "ServerControl.h"
#include "DlgCustomRule.h"

//////////////////////////////////////////////////////////////////////////


//游戏桌子类
class CTableFrameSink :public ITableFrameSink, public ITableUserAction
{
  //游戏变量
protected:
  BYTE              m_bOperaCount;              //操作次数
  WORD              m_wCurrentUser;             //当前用户
  WORD              m_wBankerUser;              //庄家用户
  WORD              m_wFlashUser[GAME_PLAYER];  //动画用户
  WORD				m_wTurnCount;				//下注轮数
  WORD				m_wMinLookTurn;				//必闷轮数
  WORD				m_wMinCompareTurn;			//比牌轮数
  WORD				m_wMaxOpenTurn;				//封顶轮数
  SCORE				m_lMaxOpenScore;			//
  BOOL				m_bSpecilBaozi;				//235＞豹子
  WORD				m_wMaxPlayerCount;			//最大人数

  BOOL				m_bCompareSmall;			//比小
  BYTE				m_cbCompareWay;				//比牌方式比大比小
  SCORE				m_lTotlalTableScore;		//总下注数

  //用户状态
protected:
  BYTE              m_cbPlayStatus[GAME_PLAYER];      //游戏状态
  BYTE              m_cbGiveUpUser[GAME_PLAYER];      //游戏状态
  LONGLONG          m_lCompareCount;				  //比牌状态
  bool              m_bGameEnd;						  //结束状态
  CMD_S_GameEnd     m_StGameEnd;					  //结束数据
  BYTE              m_cbRealPlayer[GAME_PLAYER];      //真人玩家
  BYTE              m_cbAndroidStatus[GAME_PLAYER];   //机器状态

  //扑克变量
protected:
  BYTE              m_cbHandCardData[GAME_PLAYER][MAX_COUNT];//桌面扑克
  CWHArray<WORD>    m_wCompardUser[GAME_PLAYER];			 //扑克数组

  //下注信息
protected:
  LONGLONG            m_lTableScore[GAME_PLAYER];       //下注数目
  LONGLONG            m_lUserMaxScore[GAME_PLAYER];     //最大下注
  LONGLONG            m_lMaxCellScore;            //单元上限
  LONGLONG            m_lCellScore;             //单元下注
  LONGLONG            m_lCurrentTimes;            //当前倍数
  bool				  m_bMingZhu[GAME_PLAYER];        //看明下注

  //组件变量
protected:
  CGameLogic            m_GameLogic;              //游戏逻辑
  ITableFrame           * m_pITableFrame;           //框架接口
  const tagGameServiceOption    * m_pGameServiceOption;         //配置参数

  //服务控制
protected:
  HINSTANCE           m_hControlInst;
  IServerControl*         m_pServerControl;

  //AI变量
protected:
  bool              m_bUpdataStorage;           //更新库存

  //属性变量
protected:
  static const WORD       m_wPlayerCount;             //游戏人数
  static const BYTE       m_GameStartMode;            //开始模式

  //函数定义
public:
  //构造函数
  CTableFrameSink();
  //析构函数
  virtual ~CTableFrameSink();

  //基础接口
public:
  //释放对象
  virtual VOID  Release();
  //是否有效
  virtual bool  IsValid()
  {
    return AfxIsValidAddress(this,sizeof(CTableFrameSink))?true:false;
  }
  //接口查询
  virtual void *  QueryInterface(const IID & Guid, DWORD dwQueryVer);

  //管理接口
public:
  //初始化
  virtual bool  Initialization(IUnknownEx * pIUnknownEx);
  //复位桌子
  virtual void  RepositionSink();
  //查询接口
public:
  //查询限额
  virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem)
  {
    return 0;
  }
  //最少积分
  virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem)
  {
    return 0;
  }
  //数据事件
  virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize)
  {
    return false;
  }
  //积分事件
  virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
  {
    return false;
  }
  //查询是否扣服务费
  virtual bool QueryBuckleServiceCharge(WORD wChairID)
  {
    return true;
  }

  //比赛接口
public:
  //设置基数
  virtual void SetGameBaseScore(LONG lBaseScore) {};

  //信息接口
public:
  //开始模式
  virtual BYTE GetGameStartMode();
  //游戏状态
  virtual bool IsUserPlaying(WORD wChairID);

  //游戏事件
public:
  //游戏开始
  virtual bool OnEventGameStart();
  //游戏结束
  virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
  //发送场景
  virtual bool OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret);

  virtual bool SetGameRule(byte *cbRule, int len){ 
	  PriviteRoomRule *pPriveteRoomRule = (PriviteRoomRule*)cbRule;
	  m_wMaxOpenTurn = pPriveteRoomRule->wMaxOpenTurn;
	  if (m_wMaxOpenTurn == 0)
		  m_wMaxOpenTurn = INVALID_WORD;
	  m_wMinCompareTurn = pPriveteRoomRule->wMinCompareTurn;
	  m_wMinLookTurn = pPriveteRoomRule->wMinLookTurn;
	  m_wMaxPlayerCount = pPriveteRoomRule->wMaxPlayer;
	  m_lCellScore = pPriveteRoomRule->lCellScore;
	  m_lMaxOpenScore = pPriveteRoomRule->lMaxOpenScore;
	  if (m_lMaxOpenScore == 0)
		  m_lMaxOpenScore = 0xFFFFFFFF;


	  return true; 
  }

  //事件接口
public:
  //定时器事件
  virtual bool OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
  //游戏消息处理
  virtual bool OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
  //框架消息处理
  virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);


  //用户事件
public:
  //用户断线
  virtual bool OnActionUserOffLine(WORD wChairID,IServerUserItem * pIServerUserItem)
  {
    return true;
  }
  //用户重入
  virtual bool OnActionUserConnect(WORD wChairID,IServerUserItem * pIServerUserItem)
  {
    return true;
  }
  //用户坐下
  virtual bool OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
  //用户起立
  virtual bool OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
  //用户同意
  virtual bool OnActionUserOnReady(WORD wChairID,IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize)
  {
    return true;
  }

  //游戏事件
protected:
  //放弃事件
  bool OnUserGiveUp(WORD wChairID,bool bExit=false);
  //看牌事件
  bool OnUserLookCard(WORD wChairID);
  //比牌事件
  bool OnUserCompareCard(WORD wFirstChairID,WORD wNextChairID);
  //开牌事件
  bool OnUserOpenCard(WORD wUserID);
  //加注事件
  bool OnUserAddScore(WORD wChairID, LONGLONG lScore, bool bGiveUp, bool bCompareUser);

  //功能函数
protected:
  //扑克分析
  void AnalyseStartCard();
  //是否衰减
  bool NeedDeductStorage();
  //更新房间用户信息
  void UpdateUserRosterInfo(IServerUserItem *pIServerUserItem, LONGLONG lGameScore = 0LL, USERROSTER userRoster = INVALID_ROSTER, LONGLONG lScoreLimit = 0LL);
  //解除用户名单
  void RemoveUserRoster();
  //计算机器人分数
  LONGLONG CalculateAndroidScore(WORD &wWinUser);
};

//////////////////////////////////////////////////////////////////////////

#endif
