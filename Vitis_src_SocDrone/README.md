# Vitis Workspace에서 외부 파일을 연결하는 방법
자신이 직접 Vitis Workspace 및 프로젝트를 만들고 src 폴더 내부에 git의 폴더를 연결해야 합니다.  
**중요 1: src 폴더는 Vitis에서 직접 사용하기 때문에 보존해야 합니다.**  
**중요 2: XSA 파일이 변경되면 수동으로 Platform 업데이트를 해 주어야 합니다.**  

<details>

<summary><b>외부 폴더 연결법</b></summary>

#### 1. 원하는 Vitis Workspace 준비

#### 2. Platform, System, 및 App 프로젝트 준비
<img alt="Vitis Tutorial Step 1" src = "/README_img/vitis_tutorial_1.png" height="320"/>

#### 3. "src" 폴더 내의 소스파일 (.c 및 .h) 삭제 (lscript.ld는 지우지 말 것)
<img alt="Vitis Tutorial Step 2" src = "/README_img/vitis_tutorial_2.png" height="320"/>

#### 4. "src" 폴더 우클릭 -> "New" -> "Folder" 클릭
<img alt="Vitis Tutorial Step 3" src = "/README_img/vitis_tutorial_3.png" height="320"/>

#### 5. 원하는 프로젝트 내의 "src" 폴더 선택 확인후 "Advanced" 클릭 -> "Link to alternate location" 선택 -> "Browse" 클릭
<img alt="Vitis Tutorial Step 4" src = "/README_img/vitis_tutorial_4.png" height="320"/>

#### 6. Git 폴더 내의 "git_src" 폴더 선택 후 "폴더 선택" 클릭
<img alt="Vitis Tutorial Step 5" src = "/README_img/vitis_tutorial_5.png" height="240"/>

#### 7. 선택 폴더 경로 확인 후 "Finish" 클릭
<img alt="Vitis Tutorial Step 6" src = "/README_img/vitis_tutorial_6.png" height="320"/>

#### 8. 폴더 추가 확인 및 아이콘에 화살표 표시 확인
<img alt="Vitis Tutorial Step 7" src = "/README_img/vitis_tutorial_7.png" height="320"/>

</details>


