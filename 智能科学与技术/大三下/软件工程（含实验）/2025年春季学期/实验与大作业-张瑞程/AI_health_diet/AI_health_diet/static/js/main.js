// 这个文件将用于添加额外的CSS动画和交互效果
document.addEventListener('DOMContentLoaded', function() {
    // 滚动动画
    const animateOnScrollElements = document.querySelectorAll('.animate-on-scroll');
    
    const observer = new IntersectionObserver((entries) => {
        entries.forEach(entry => {
            if (entry.isIntersecting) {
                entry.target.classList.add('visible');
            }
        });
    }, {threshold: 0.1});
    
    animateOnScrollElements.forEach(el => {
        observer.observe(el);
    });
    
    // 移动端菜单切换
    const menuToggle = document.querySelector('.mobile-menu-toggle');
    const navMenu = document.querySelector('.nav-menu');
    
    if (menuToggle) {
        menuToggle.addEventListener('click', function() {
            this.classList.toggle('active');
            navMenu.classList.toggle('active');
        });
    }
    
    // 平滑滚动到锚点
    document.querySelectorAll('a[href^="#"]').forEach(anchor => {
        anchor.addEventListener('click', function(e) {
            e.preventDefault();
            const targetId = this.getAttribute('href');
            if (targetId !== '#') {
                const targetElement = document.querySelector(targetId);
                if (targetElement) {
                    window.scrollTo({
                        top: targetElement.offsetTop - 80, // 考虑导航栏高度
                        behavior: 'smooth'
                    });
                }
            }
        });
    });

    // 回到顶部按钮
    const backToTopBtn = document.getElementById('backToTop');
    if (backToTopBtn) {
        window.addEventListener('scroll', function() {
            if (window.scrollY > 300) {
                backToTopBtn.classList.add('show');
            } else {
                backToTopBtn.classList.remove('show');
            }
        });
        
        backToTopBtn.addEventListener('click', function() {
            window.scrollTo({
                top: 0,
                behavior: 'smooth'
            });
        });
    }
    
    // 加载动画控制
    const loadingControls = document.querySelectorAll('.loading-control');
    loadingControls.forEach(control => {
        control.addEventListener('click', function() {
            const targetId = this.getAttribute('data-target');
            const loadingElement = document.getElementById(targetId);
            if (loadingElement) {
                loadingElement.style.display = 'block';
                const form = this.closest('form');
                if (form) {
                    form.classList.add('loading');
                }
            }
        });
    });
});

// 表单相关函数
function validateForm(formId) {
    const form = document.getElementById(formId);
    if (!form) return true;
    
    let isValid = true;
    const requiredFields = form.querySelectorAll('[required]');
    
    requiredFields.forEach(field => {
        if (!field.value.trim()) {
            isValid = false;
            field.classList.add('error');
            
            const errorMsg = field.getAttribute('data-error') || '此字段不能为空';
            let errorElement = field.nextElementSibling;
            
            if (!errorElement || !errorElement.classList.contains('error-message')) {
                errorElement = document.createElement('div');
                errorElement.className = 'error-message';
                field.parentNode.insertBefore(errorElement, field.nextSibling);
            }
            
            errorElement.textContent = errorMsg;
        } else {
            field.classList.remove('error');
            const errorElement = field.nextElementSibling;
            if (errorElement && errorElement.classList.contains('error-message')) {
                errorElement.textContent = '';
            }
        }
    });
    
    return isValid;
}

function saveUserPreferences() {
    const userPrefForm = document.getElementById('userPreferencesForm');
    if (!userPrefForm) return;
    
    const formData = new FormData(userPrefForm);
    const userData = {};
    
    for(const [key, value] of formData.entries()) {
        userData[key] = value;
    }
    
    localStorage.setItem('userPreferences', JSON.stringify(userData));
}

function loadUserPreferences() {
    const userPrefString = localStorage.getItem('userPreferences');
    if (!userPrefString) return;
    
    const userData = JSON.parse(userPrefString);
    const userPrefForm = document.getElementById('userPreferencesForm');
    if (!userPrefForm) return;
    
    for(const key in userData) {
        const field = userPrefForm.querySelector(`[name="${key}"]`);
        if (field) {
            field.value = userData[key];
        }
    }
    
    // 触发BMI计算等
    if (typeof calculateBMI === 'function') {
        calculateBMI();
    }
}

// 计算BMI
function calculateBMI() {
    const heightField = document.getElementById('height');
    const weightField = document.getElementById('weight');
    const bmiValueElement = document.getElementById('bmiValue');
    const bmiStatusElement = document.getElementById('bmiStatus');
    
    if (!heightField || !weightField || !bmiValueElement || !bmiStatusElement) return;
    
    const height = parseFloat(heightField.value) / 100; // 转换为米
    const weight = parseFloat(weightField.value);
    
    if (isNaN(height) || isNaN(weight) || height <= 0 || weight <= 0) {
        bmiValueElement.textContent = "-";
        bmiStatusElement.textContent = "请输入有效的身高和体重";
        return;
    }
    
    const bmi = weight / (height * height);
    bmiValueElement.textContent = bmi.toFixed(1);
    
    let status = "";
    let statusClass = "";
    
    if (bmi < 18.5) {
        status = "体重过轻";
        statusClass = "status-underweight";
    } else if (bmi < 24) {
        status = "体重正常";
        statusClass = "status-normal";
    } else if (bmi < 28) {
        status = "超重";
        statusClass = "status-overweight";
    } else {
        status = "肥胖";
        statusClass = "status-obese";
    }
    
    bmiStatusElement.textContent = status;
    bmiStatusElement.className = statusClass;
}
