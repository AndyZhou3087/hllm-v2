<GameFile>
  <PropertyGroup Name="FailLayer" Type="Layer" ID="86a157fb-6476-4222-8015-31aba32f275a" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" Tag="13" ctype="GameLayerObjectData">
        <Size X="540.0000" Y="960.0000" />
        <Children>
          <AbstractNodeData Name="FailSureLayerBg" ActionTag="951207889" VisibleForFrame="False" Tag="14" IconVisible="False" TopMargin="290.0000" BottomMargin="290.0000" ctype="SpriteObjectData">
            <Size X="540.0000" Y="380.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="270.0000" Y="480.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.0000" Y="0.3958" />
            <FileData Type="MarkedSubImage" Path="UI/failbg.png" Plist="ui.plist" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="leaf2_left" ActionTag="1174296621" VisibleForFrame="False" Tag="218" IconVisible="False" LeftMargin="135.5203" RightMargin="355.4797" TopMargin="243.4998" BottomMargin="643.5002" ctype="SpriteObjectData">
            <Size X="49.0000" Y="73.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="160.0203" Y="680.0002" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.2963" Y="0.7083" />
            <PreSize X="0.0907" Y="0.0760" />
            <FileData Type="MarkedSubImage" Path="UI/leaf2.png" Plist="ui.plist" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="leaf2_right" ActionTag="-978483976" VisibleForFrame="False" Tag="219" IconVisible="False" LeftMargin="338.0844" RightMargin="152.9156" TopMargin="243.4998" BottomMargin="643.5002" FlipX="True" ctype="SpriteObjectData">
            <Size X="49.0000" Y="73.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="362.5844" Y="680.0002" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.6715" Y="0.7083" />
            <PreSize X="0.0907" Y="0.0760" />
            <FileData Type="MarkedSubImage" Path="UI/leaf2.png" Plist="ui.plist" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="tie" ActionTag="1548279307" VisibleForFrame="False" Tag="423" IconVisible="False" LeftMargin="168.4993" RightMargin="184.5007" TopMargin="243.5018" BottomMargin="637.4982" ctype="SpriteObjectData">
            <Size X="187.0000" Y="79.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="261.9993" Y="676.9982" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.4852" Y="0.7052" />
            <PreSize X="0.3463" Y="0.0823" />
            <FileData Type="MarkedSubImage" Path="UI/tie.png" Plist="ui.plist" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="failCloseBtn" ActionTag="1509566473" Tag="278" IconVisible="False" LeftMargin="469.7486" RightMargin="9.2514" TopMargin="282.6042" BottomMargin="612.3958" TouchEnable="True" FontSize="14" Scale9Enable="True" Scale9Width="61" Scale9Height="65" OutlineSize="0" ShadowOffsetX="0.0000" ShadowOffsetY="0.0000" ctype="ButtonObjectData">
            <Size X="61.0000" Y="65.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="500.2486" Y="644.8958" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.9264" Y="0.6718" />
            <PreSize X="0.1130" Y="0.0677" />
            <TextColor A="255" R="65" G="65" B="70" />
            <DisabledFileData Type="MarkedSubImage" Path="UI/close.png" Plist="ui.plist" />
            <PressedFileData Type="MarkedSubImage" Path="UI/close.png" Plist="ui.plist" />
            <NormalFileData Type="MarkedSubImage" Path="UI/close.png" Plist="ui.plist" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="tipsbmlbl" ActionTag="1199010912" VisibleForFrame="False" Tag="284" IconVisible="False" LeftMargin="-37.9999" RightMargin="-38.0001" TopMargin="621.5000" BottomMargin="307.5000" LabelText="“自动提示”是个不错的技能，快去升级角色学习技能吧~" ctype="TextBMFontObjectData">
            <Size X="616.0000" Y="31.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="270.0001" Y="323.0000" />
            <Scale ScaleX="0.8000" ScaleY="0.8000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.3365" />
            <PreSize X="1.1407" Y="0.0323" />
            <LabelBMFontFile_CNB Type="Normal" Path="fonts/tips.fnt" Plist="" />
          </AbstractNodeData>
          <AbstractNodeData Name="failbosstitle" ActionTag="201288375" VisibleForFrame="False" Tag="409" IconVisible="False" LeftMargin="148.5000" RightMargin="168.5000" TopMargin="242.0000" BottomMargin="642.0000" ctype="SpriteObjectData">
            <Size X="223.0000" Y="76.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="260.0000" Y="680.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.4815" Y="0.7083" />
            <PreSize X="0.4130" Y="0.0792" />
            <FileData Type="MarkedSubImage" Path="UI/failbosstitle.png" Plist="ui.plist" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="onsalebtn" ActionTag="1937138410" Tag="463" IconVisible="False" LeftMargin="292.3642" RightMargin="37.6358" TopMargin="705.9999" BottomMargin="162.0001" TouchEnable="True" FontSize="14" Scale9Enable="True" Scale9Width="201" Scale9Height="92" OutlineSize="0" ShadowOffsetX="0.0000" ShadowOffsetY="0.0000" ctype="ButtonObjectData">
            <Size X="210.0000" Y="92.0000" />
            <Children>
              <AbstractNodeData Name="text" ActionTag="-2026727701" Tag="464" IconVisible="False" LeftMargin="47.5838" RightMargin="49.4162" TopMargin="25.5000" BottomMargin="37.5000" ctype="SpriteObjectData">
                <Size X="113.0000" Y="29.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="104.0838" Y="52.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.4956" Y="0.5652" />
                <PreSize X="0.5381" Y="0.3152" />
                <FileData Type="MarkedSubImage" Path="UI/tgmj.png" Plist="ui.plist" />
                <BlendFunc Src="1" Dst="771" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="397.3642" Y="208.0001" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.7359" Y="0.2167" />
            <PreSize X="0.3889" Y="0.0958" />
            <TextColor A="255" R="65" G="65" B="70" />
            <DisabledFileData Type="MarkedSubImage" Path="UI/yellowbtn.png" Plist="ui.plist" />
            <PressedFileData Type="MarkedSubImage" Path="UI/yellowbtn.png" Plist="ui.plist" />
            <NormalFileData Type="MarkedSubImage" Path="UI/yellowbtn.png" Plist="ui.plist" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="restartbtn" ActionTag="1325740680" Tag="290" IconVisible="False" LeftMargin="43.9774" RightMargin="286.0226" TopMargin="706.5000" BottomMargin="166.5000" TouchEnable="True" FontSize="14" Scale9Enable="True" Scale9Width="242" Scale9Height="87" OutlineSize="0" ShadowOffsetX="0.0000" ShadowOffsetY="0.0000" ctype="ButtonObjectData">
            <Size X="210.0000" Y="87.0000" />
            <Children>
              <AbstractNodeData Name="usepropbox" ActionTag="2053846294" VisibleForFrame="False" Tag="291" IconVisible="False" LeftMargin="23.5000" RightMargin="143.5000" TopMargin="18.0000" BottomMargin="25.0000" ctype="SpriteObjectData">
                <Size X="43.0000" Y="44.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="45.0000" Y="47.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.2143" Y="0.5402" />
                <PreSize X="0.2048" Y="0.5057" />
                <FileData Type="MarkedSubImage" Path="UI/usepropbox.png" Plist="ui.plist" />
                <BlendFunc Src="1" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="hpicon" ActionTag="-495572856" VisibleForFrame="False" Tag="292" IconVisible="False" LeftMargin="13.1291" RightMargin="138.8709" TopMargin="10.8380" BottomMargin="23.1620" ctype="SpriteObjectData">
                <Size X="58.0000" Y="53.0000" />
                <AnchorPoint ScaleX="0.5664" ScaleY="0.5350" />
                <Position X="45.9803" Y="51.5170" />
                <Scale ScaleX="0.4000" ScaleY="0.4000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.2190" Y="0.5921" />
                <PreSize X="0.2762" Y="0.6092" />
                <FileData Type="MarkedSubImage" Path="UI/hp.png" Plist="ui.plist" />
                <BlendFunc Src="1" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="restarttext" ActionTag="-891777005" Tag="293" IconVisible="False" LeftMargin="52.4983" RightMargin="46.5017" TopMargin="23.9989" BottomMargin="33.0011" ctype="SpriteObjectData">
                <Size X="111.0000" Y="30.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="107.9983" Y="48.0011" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5143" Y="0.5517" />
                <PreSize X="0.5286" Y="0.3448" />
                <FileData Type="MarkedSubImage" Path="UI/restarttext.png" Plist="ui.plist" />
                <BlendFunc Src="1" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="usehpbmlbl" ActionTag="-2013814345" VisibleForFrame="False" Tag="294" IconVisible="False" LeftMargin="28.6244" RightMargin="153.3756" TopMargin="41.4463" BottomMargin="13.5537" LabelText="x2" ctype="TextBMFontObjectData">
                <Size X="28.0000" Y="32.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="42.6244" Y="29.5537" />
                <Scale ScaleX="0.7000" ScaleY="0.7000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.2030" Y="0.3397" />
                <PreSize X="0.1333" Y="0.3678" />
                <LabelBMFontFile_CNB Type="Normal" Path="fonts/buynum.fnt" Plist="" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="148.9774" Y="210.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.2759" Y="0.2188" />
            <PreSize X="0.3889" Y="0.0906" />
            <TextColor A="255" R="65" G="65" B="70" />
            <DisabledFileData Type="MarkedSubImage" Path="UI/okbtn.png" Plist="ui.plist" />
            <PressedFileData Type="MarkedSubImage" Path="UI/okbtn.png" Plist="ui.plist" />
            <NormalFileData Type="MarkedSubImage" Path="UI/okbtn.png" Plist="ui.plist" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="surestagebmlbl" ActionTag="435340903" VisibleForFrame="False" Tag="253" IconVisible="False" LeftMargin="241.1982" RightMargin="256.8018" TopMargin="266.1461" BottomMargin="662.8539" LabelText="14" ctype="TextBMFontObjectData">
            <Size X="42.0000" Y="31.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="262.1982" Y="678.3539" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.4856" Y="0.7066" />
            <PreSize X="0.0778" Y="0.0323" />
            <LabelBMFontFile_CNB Type="Normal" Path="fonts/surestagenum.fnt" Plist="" />
          </AbstractNodeData>
          <AbstractNodeData Name="suredi" ActionTag="-1522716508" VisibleForFrame="False" Tag="254" IconVisible="False" LeftMargin="195.7027" RightMargin="211.2973" TopMargin="261.6395" BottomMargin="660.3605" ctype="SpriteObjectData">
            <Size X="133.0000" Y="38.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="262.2027" Y="679.3605" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.4856" Y="0.7077" />
            <PreSize X="0.2463" Y="0.0396" />
            <FileData Type="MarkedSubImage" Path="UI/suredi.png" Plist="ui.plist" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>