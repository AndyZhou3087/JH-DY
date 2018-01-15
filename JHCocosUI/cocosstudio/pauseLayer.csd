<GameFile>
  <PropertyGroup Name="pauseLayer" Type="Layer" ID="b4824830-a1cf-4633-ad4b-b9f14ad3f0af" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" Tag="200" ctype="GameLayerObjectData">
        <Size X="720.0000" Y="1280.0000" />
        <Children>
          <AbstractNodeData Name="bg" ActionTag="328469804" Tag="206" IconVisible="False" LeftMargin="94.9800" RightMargin="102.0200" TopMargin="362.0000" BottomMargin="362.0000" Scale9Enable="True" LeftEage="172" RightEage="172" TopEage="183" BottomEage="183" Scale9OriginX="172" Scale9OriginY="183" Scale9Width="179" Scale9Height="190" ctype="ImageViewObjectData">
            <Size X="523.0000" Y="556.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="356.4800" Y="640.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.4951" Y="0.5000" />
            <PreSize X="0.7264" Y="0.4344" />
            <FileData Type="Normal" Path="images/syssmallbg.png" Plist="" />
          </AbstractNodeData>
          <AbstractNodeData Name="title" ActionTag="-516988748" Tag="203" IconVisible="False" LeftMargin="313.1481" RightMargin="319.8519" TopMargin="420.0816" BottomMargin="810.9184" FontSize="45" LabelText="暂停" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
            <Size X="87.0000" Y="49.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="356.6481" Y="835.4184" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="0" G="0" B="0" />
            <PrePosition X="0.4953" Y="0.6527" />
            <PreSize X="0.1208" Y="0.0383" />
            <FontResource Type="Normal" Path="fonts/STXINGKA.TTF" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="syssmalltie_left" ActionTag="-750866858" Tag="204" IconVisible="False" LeftMargin="131.4486" RightMargin="408.5514" TopMargin="424.7102" BottomMargin="817.2898" ctype="SpriteObjectData">
            <Size X="180.0000" Y="38.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="221.4486" Y="836.2898" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.3076" Y="0.6534" />
            <PreSize X="0.2500" Y="0.0297" />
            <FileData Type="MarkedSubImage" Path="ui/syssmalltie.png" Plist="uipacker.plist" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="syssmalltie_right" ActionTag="1723358487" Tag="205" IconVisible="False" LeftMargin="404.7111" RightMargin="135.2889" TopMargin="424.7102" BottomMargin="817.2898" FlipX="True" ctype="SpriteObjectData">
            <Size X="180.0000" Y="38.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="494.7111" Y="836.2898" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.6871" Y="0.6534" />
            <PreSize X="0.2500" Y="0.0297" />
            <FileData Type="MarkedSubImage" Path="ui/syssmalltie.png" Plist="uipacker.plist" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="continuebtn" ActionTag="-33922216" Tag="207" IconVisible="False" LeftMargin="159.0014" RightMargin="158.9986" TopMargin="532.8549" BottomMargin="661.1451" TouchEnable="True" FontSize="50" ButtonText="继续游戏" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="372" Scale9Height="64" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
            <Size X="402.0000" Y="86.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="360.0014" Y="704.1451" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5501" />
            <PreSize X="0.5583" Y="0.0672" />
            <FontResource Type="Normal" Path="fonts/STXINGKA.TTF" Plist="" />
            <TextColor A="255" R="255" G="255" B="255" />
            <DisabledFileData Type="MarkedSubImage" Path="ui/startbtndis.png" Plist="uipacker.plist" />
            <PressedFileData Type="MarkedSubImage" Path="ui/startbtn.png" Plist="uipacker.plist" />
            <NormalFileData Type="MarkedSubImage" Path="ui/startbtn.png" Plist="uipacker.plist" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="gohomebtn" ActionTag="-733047565" Tag="208" IconVisible="False" LeftMargin="153.7109" RightMargin="164.2891" TopMargin="692.3571" BottomMargin="501.6429" TouchEnable="True" FontSize="50" ButtonText="返回主页" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="372" Scale9Height="64" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
            <Size X="402.0000" Y="86.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="354.7109" Y="544.6429" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.4927" Y="0.4255" />
            <PreSize X="0.5583" Y="0.0672" />
            <FontResource Type="Normal" Path="fonts/STXINGKA.TTF" Plist="" />
            <TextColor A="255" R="255" G="255" B="255" />
            <DisabledFileData Type="MarkedSubImage" Path="ui/startbtn.png" Plist="uipacker.plist" />
            <PressedFileData Type="MarkedSubImage" Path="ui/startbtn.png" Plist="uipacker.plist" />
            <NormalFileData Type="MarkedSubImage" Path="ui/startbtn.png" Plist="uipacker.plist" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>