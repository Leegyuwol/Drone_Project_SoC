# Git 이용한 Vivado 프로젝트 사용법
리포지토리에는 소스파일만 보관하기 위해 프로젝트와 소스파일을 분리하였습니다.  
따라서 Vivado 프로젝트에서 외부의 파일을 연결해 주어야 합니다.  
또한 커스텀 IP가 있는 리포지토리 경로도 추가해 주어야 합니다.  

<details>
<summary><b>(클릭) 1. 외부 파일 연결하기</b></summary>

#### 1-1. 원하는 비바도 프로젝트 생성 또는 이미 생성된 프로젝트 열기

#### 1-2. 비바도 왼쪽의 "Add Sources" 클릭
<img alt="Vivado Link File Tutorial Step 1" src = "/README_img/vivado_linkfile_tutorial_1.png" height="320"/>

#### 1-3. "Add or create design sources" 선택 후 "Next" 클릭
<img alt="Vivado Link File Tutorial Step 2" src = "/README_img/vivado_linkfile_tutorial_2.png" height="320"/>

#### 1-4. "Add Files" 클릭
<img alt="Vivado Link File Tutorial Step 3" src = "/README_img/vivado_linkfile_tutorial_3.png" height="320"/>

#### 1-5. 리포지토리 내의 소스 파일 선택 후 "OK" 클릭
Block Design의 경우에는 .bd 파일 선택 <br>
Verilog 파일의 경우에는 .v 파일 선택 <br><br>
<img alt="Vivado Link File Tutorial Step 4" src = "/README_img/vivado_linkfile_tutorial_4.png" height="320"/>

#### 1-6. "Copy sources into project" 체크 해제 후 "Finish" 클릭
**중요함: 해당 체크 박스를 해제해야만 리포지토리 내의 파일을 직접 사용함**<br><br>
<img alt="Vivado Link File Tutorial Step 5" src = "/README_img/vivado_linkfile_tutorial_5.png" height="320"/>

<br><br>
위 작업 후에 Vivado 프로젝트와 소스파일이 별도로 관리됩니다.

</details>

<details>
<summary><b>(클릭) 2. IP 리포지토리 연결하기</b></summary>

#### 2-1. 프로젝트 왼편의 "Settings" 클릭
<img alt="Vivado Link Repo Tutorial Step 5" src = "/README_img/vivado_linkrepo_tutorial_1.png" height="320"/>

#### 2-2. "Project Settings" -> "IP" 옆 ">" 클릭하여 목록 확장
<img alt="Vivado Link Repo Tutorial Step 5" src = "/README_img/vivado_linkrepo_tutorial_2.png" height="320"/>

#### 2-3. "Project Settings" -> "IP" -> "Repository" 클릭
<img alt="Vivado Link Repo Tutorial Step 5" src = "/README_img/vivado_linkrepo_tutorial_3.png" height="320"/>

#### 2-4. "IP Repositories" 밑 "+" 클릭
<img alt="Vivado Link Repo Tutorial Step 5" src = "/README_img/vivado_linkrepo_tutorial_4.png" height="320"/>

#### 2-5. Git 리포지토리 내부의 "ip_repo" 폴더 찾아서 선택 뒤 "Select" 클릭
<img alt="Vivado Link Repo Tutorial Step 5" src = "/README_img/vivado_linkrepo_tutorial_5.png" height="320"/>

#### 2-6. 리포지토리 추가 확인 후 "OK" 클릭
<img alt="Vivado Link Repo Tutorial Step 5" src = "/README_img/vivado_linkrepo_tutorial_6.png" height="320"/>

#### 2-7. 리포지토리 경로 확인 후 "OK" 클릭
<img alt="Vivado Link Repo Tutorial Step 5" src = "/README_img/vivado_linkrepo_tutorial_7.png" height="320"/>

위 작업 후에 Vivado 프로젝트에 IP 리포지토리 경로가 추가됩니다.

</details>