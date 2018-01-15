#include "CommonFuncs.h"
#include "Const.h"
#include "SoundManager.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "../cocos2d//external//win32-specific/icon/include/iconv.h"
#endif

#define Button_ACTION (Sequence::create(ScaleTo::create(0.05f,0.92f),NULL))
#define Button_ACTION1 (Sequence::create(ScaleTo::create(0.05f,1),NULL))

int CommonFuncs::split(const std::string& str, std::vector<std::string>& ret_, std::string sep)
{
	if (str.empty())
	{
		return 0;
	}

	std::string tmp;
	std::string::size_type pos_begin = 0;//str.find_first_not_of(sep);
	std::string::size_type comma_pos = 0;

	while (pos_begin != std::string::npos)
	{
		comma_pos = str.find(sep, pos_begin);
		if (comma_pos != std::string::npos)
		{
			tmp = str.substr(pos_begin, comma_pos - pos_begin);
			pos_begin = comma_pos + sep.length();
		}
		else
		{
			tmp = str.substr(pos_begin);
			pos_begin = comma_pos;
		}

		//if (!tmp.empty())
		//{
			ret_.push_back(tmp);
			tmp.clear();
		//}
	}

	return 0;
}

std::string CommonFuncs::replace_all(std::string & s, std::string const & t, std::string const & w)
{
	std::string::size_type pos = s.find(t), t_size = t.size(), r_size = w.size();
	while (pos != std::string::npos){ // found 
		s.replace(pos, t_size, w);
		pos = s.find(t, pos + r_size);
	}
	return s;
}

int CommonFuncs::code_convert(const char *from_charset, const char *to_charset, const char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{


#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	iconv_t cd;
	const char *temp = inbuf;
	const char **pin = &temp;
	char **pout = &outbuf;
	memset(outbuf, 0, outlen);
    cd = iconv_open(to_charset, from_charset);
	if (cd == 0) return -1;
	if (iconv(cd, pin, &inlen, pout, &outlen) == -1) return -1;
	iconv_close(cd);
#else
//    iconv_t cd;
//    cd = iconv_open(to_charset, from_charset);
//	if (iconv(cd, &inbuf, &inlen, &outbuf, &outlen) == -1)
//	{
//		iconv_close(cd);
//		return  - 1;
//	}
	
#endif
	return 0;
}

std::string CommonFuncs::gbk2utf(const char *inbuf)
{
	std::string strRet;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	size_t inlen = strlen(inbuf);
	char * outbuf = new char[inlen * 2 + 2];

	if (code_convert("gb2312", "utf-8", inbuf, inlen, outbuf, inlen * 2 + 2) == 0)
	{
		strRet = outbuf;
	}
	delete[] outbuf;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	strRet = inbuf;
#endif
	return strRet;
}


void CommonFuncs::BtnAction(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == cocos2d::ui::Widget::TouchEventType::BEGAN || type == cocos2d::ui::Widget::TouchEventType::MOVED)
	{
		((cocos2d::ui::Widget*)pSender)->runAction(Button_ACTION);
	}
	else if (type == cocos2d::ui::Widget::TouchEventType::CANCELED)
	{
		((cocos2d::ui::Widget*)pSender)->runAction(Button_ACTION1);
	}
	else if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		((cocos2d::ui::Widget*)pSender)->runAction(Button_ACTION1);
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
	}
}

void CommonFuncs::changeGray(cocos2d::Node* node)
{
	auto p = GLProgram::createWithFilenames("gray.vsh", "gray.fsh");
	p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
	p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
	p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);
	p->link();
	p->updateUniforms();

	node->setGLProgram(p);
}

void CommonFuncs::removeGray(cocos2d::Node* node)
{
	std::string str = GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP;//ShaderPositionTextureColor_noMVP;
	GLProgram * pProgram = ShaderCache::getInstance()->getGLProgram(str);
	node->setGLProgram(pProgram);
}
