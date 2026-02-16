# Unreal Engine Gameplay Systems 
## <u> Chapters </u>:

1. [Few Words About Me](#few-words-about-me) 
   -  [Few Things About this Document](#few-things-about-this-document)
2. [Targeting System](#targeting-system)
   - [Blueprint Approach](#blueprint-approach)












#### Few Words About Me 
---
<p align="justify">
 Hello, my name is Charalampos and i am currently trying to transition from an Accounting and Finance backround, to create and design Gameplay Systems in Unreal Engine. 
</p>

<p align="justify">
 I’m not a professional programmer yet, but I understand basic <b>C++</b> and <b>Blueprint</b> scripting. I’m extremely curious and enjoy researching and building systems from scratch. I am currently document myself making several systems inside Unreal Engine here:
</p>

:point_right: <https://www.youtube.com/@IsHeReally98> (ignore the poor quality :disappointed:) . 

For now i've made some videos about:
- Targeting System,
- Input System
- Drop/Loot System (My own version & With Unreal Engine Asset Manager)
- Inventory System
- Equipment
- GAS 

<p align="justify">
I plan to document those systems as well, as much as i can, but my current goal is to learn not only how blueprints interact with each other and C++ but how can i optimaze them as much as i can.
</p>
<p align="justify">
Anyways the point, of this document, or rather the theme of this document will be the <b>Targeting System</b>, since this was my first project as well.
</p>


#### Few Things About This Document
---
When I first started learning Unreal Engine, I relied heavily on courses and tutorials and fell into what many call *tutorial hell*.

Over the last few months I decided to break away from copying and instead build my own systems. This document reflects that shift.

The targeting system here allows the player to cycle through targets by pressing a key. The implementation is presented in three stages:

- Blueprint-only approach
- Hybrid Blueprint + C++
- Full C++ implementation

The goal is to understand the logic, not just reproduce results.

#### Targeting System 
---
What is a targeting system?
<p align="justify">
Different genres approach targeting differently. A turn-based game, for example, selects a target directly through UI interaction, so there’s no need for runtime cycling or scanning.</p>
<p align="justify">
In real-time single-player or multiplayer games, however, the system must continuously compute valid targets around the player. That is where targeting logic becomes essential.</p>
<p align="justify">
And since my own project, are based on this type of genre this <b>Targeting System</b> will be based on cycling, sorted by <b>Distance, Sight</b> and <b>Enemy Types</b>.
</p>

#### Blueprint Approach
---
<p align="justify">
 My first ever video, of me documenting myself was based on a <b>Targeting System</b> that the Player could use, to cycle through different enemies. For example, i wanted the player to be able to target an Enemy, mostly an AI since this project focuses on single player perspective, and no matter the number of enemies in the scene, could cycle through and be sorted by <b>Distance, Sight</b> and <b>Enemy Types</b>. I actually managed to do that but now that i look back on how the project is, i feel like it's not only unoptimazed but the scenario to be able to target a ''large'' amount of Enemies is not ideal.
</p>

:point_right: <https://www.youtube.com/watch?v=0PXnXTQs_8o&t=1265s>

<p align="justify">
 In the video above, i try my best to explain what i am essentialy doing, but some <b>key parts</b> are, that i use local <b>Blueprint</b> TArray containers through the functions to collect correct data (Actor Pointers), then loop over them with the <b>for each Blueprint </b> node and sort them by <b>Sight</b> and then by <b>Distance</b>, using specifically the functions <b>Overlap Actors</b> and <b>Single Line Trace For Objects</b>. </p>

[look the pictures below](#pictures)



---

<a id="pictures"></a>

| | |
|-|-|
| <a href="ScreenShots/CancelTargeting.png"><img src="ScreenShots/CancelTargeting.png" width="300"/></a> | <a href="ScreenShots/CheckArrays.PNG"><img src="ScreenShots/CheckArrays.png" width="300"/></a> |
