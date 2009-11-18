#include "TextRenderer.h"

template<> TextRenderer* Ogre::Singleton<TextRenderer>::ms_Singleton = 0;

TextRenderer::TextRenderer()
{
	_overlayMgr = Ogre::OverlayManager::getSingletonPtr();

	_overlay = _overlayMgr->create("banana");
	_panel = static_cast<Ogre::OverlayContainer*>(_overlayMgr->createOverlayElement("Panel", "1"));
	_panel->setDimensions(10000000000000000000, 10000000000000000000);
	_panel->setPosition(0, 0);
	_panel->setMaterialName("portal/portal");

	_overlay->add2D(_panel);
	_overlay->setScale(0.0000000001,0.000000000001);

	//Uncomment for nice gray screen.
	_overlay->show();
}

void TextRenderer::addTextBox(
							  const std::string& ID,
							  const std::string& text,
							  Ogre::Real x, Ogre::Real y,
							  Ogre::Real width, Ogre::Real height,
							  const Ogre::ColourValue& color)
{
	Ogre::OverlayElement* textBox = _overlayMgr->createOverlayElement("TextArea", ID);
	textBox->setDimensions(width, height);
	textBox->setMetricsMode(Ogre::GMM_PIXELS);
	textBox->setPosition(x, y);
	textBox->setWidth(width);
	textBox->setHeight(height);
	textBox->setParameter("char_height", "16");
	textBox->setColour(color);

	textBox->setCaption(text);

	_panel->addChild(textBox);
}

void TextRenderer::removeTextBox(const std::string& ID)
{
	_panel->removeChild(ID);
	_overlayMgr->destroyOverlayElement(ID);
}

void TextRenderer::setText(const std::string& ID, const std::string& Text)
{
	Ogre::OverlayElement* textBox = _overlayMgr->getOverlayElement(ID);
	textBox->setCaption(Text);
}