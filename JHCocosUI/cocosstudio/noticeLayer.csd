<GameFile>
  <PropertyGroup Name="noticeLayer" Type="Layer" ID="f309534c-56a9-45b6-bb21-cd7e7428fc6b" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" Tag="13" ctype="GameLayerObjectData">
        <Size X="720.0000" Y="1280.0000" />
        <Children>
          <AbstractNodeData Name="noticebg" ActionTag="1133844965" Tag="14" IconVisible="False" TopMargin="220.0000" BottomMargin="220.0000" ctype="SpriteObjectData">
            <Size X="720.0000" Y="840.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="360.0000" Y="640.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.0000" Y="0.6563" />
            <FileData Type="Normal" Path="images/noticebg.png" Plist="" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="title" ActionTag="1607239592" Tag="45" IconVisible="False" LeftMargin="302.0000" RightMargin="302.0000" TopMargin="233.0000" BottomMargin="985.0000" FontSize="60" LabelText="告示" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
            <Size X="116.0000" Y="62.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="360.0000" Y="1016.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="0" G="0" B="0" />
            <PrePosition X="0.5000" Y="0.7937" />
            <PreSize X="0.1611" Y="0.0484" />
            <FontResource Type="Normal" Path="fonts/STXINGKA.TTF" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="okbtn" ActionTag="-505542214" Tag="83" IconVisible="False" LeftMargin="285.0000" RightMargin="285.0000" TopMargin="873.7906" BottomMargin="316.2094" TouchEnable="True" FontSize="40" ButtonText="知道了" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="120" Scale9Height="68" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
            <Size X="150.0000" Y="90.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="360.0000" Y="361.2094" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.2822" />
            <PreSize X="0.2083" Y="0.0703" />
            <FontResource Type="Normal" Path="fonts/STXINGKA.TTF" Plist="" />
            <TextColor A="255" R="255" G="255" B="255" />
            <DisabledFileData Type="MarkedSubImage" Path="ui/normalbtn.png" Plist="uipacker.plist" />
            <PressedFileData Type="MarkedSubImage" Path="ui/normalbtn.png" Plist="uipacker.plist" />
            <NormalFileData Type="MarkedSubImage" Path="ui/normalbtn.png" Plist="uipacker.plist" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="desc" ActionTag="-1917809870" Tag="101" IconVisible="False" LeftMargin="499.2473" RightMargin="53.7527" TopMargin="966.2976" BottomMargin="284.7024" FontSize="28" LabelText="今日不再弹出" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
            <Size X="167.0000" Y="29.0000" />
            <AnchorPoint ScaleY="0.5000" />
            <Position X="499.2473" Y="299.2024" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="0" G="0" B="0" />
            <PrePosition X="0.6934" Y="0.2338" />
            <PreSize X="0.2319" Y="0.0227" />
            <FontResource Type="Normal" Path="fonts/STXINGKA.TTF" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="check" ActionTag="-968576151" Tag="102" IconVisible="False" LeftMargin="453.6138" RightMargin="221.3862" TopMargin="957.2742" BottomMargin="278.7258" TouchEnable="True" ctype="CheckBoxObjectData">
            <Size X="45.0000" Y="44.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="476.1138" Y="300.7258" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.6613" Y="0.2349" />
            <PreSize X="0.0625" Y="0.0344" />
            <NormalBackFileData Type="MarkedSubImage" Path="ui/settingselectbox.png" Plist="uipacker.plist" />
            <PressedBackFileData Type="MarkedSubImage" Path="ui/settingselectbox.png" Plist="uipacker.plist" />
            <DisableBackFileData Type="MarkedSubImage" Path="ui/settingselectbox.png" Plist="uipacker.plist" />
            <NodeNormalFileData Type="MarkedSubImage" Path="ui/settingselect.png" Plist="uipacker.plist" />
            <NodeDisableFileData Type="MarkedSubImage" Path="ui/settingselect.png" Plist="uipacker.plist" />
          </AbstractNodeData>
          <AbstractNodeData Name="ScrollView" ActionTag="-1791717748" Tag="111" IconVisible="False" LeftMargin="39.9982" RightMargin="30.0018" TopMargin="362.0003" BottomMargin="417.9997" TouchEnable="True" ClipAble="True" BackColorAlpha="0" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" IsBounceEnabled="True" ScrollDirectionType="Vertical" ctype="ScrollViewObjectData">
            <Size X="650.0000" Y="500.0000" />
            <AnchorPoint />
            <Position X="39.9982" Y="417.9997" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.0556" Y="0.3266" />
            <PreSize X="0.9028" Y="0.3906" />
            <SingleColor A="255" R="255" G="150" B="100" />
            <FirstColor A="255" R="255" G="150" B="100" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
            <InnerNodeSize Width="650" Height="500" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>