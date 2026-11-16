<!DOCTYPE html>
<html lang="ru">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Дурак - Пермское Казино</title>
    <style>
        :root {
            --bg-color: #0f172a;
            --card-bg: #1e293b;
            --accent: #eab308;
            --text: #f8fafc;
            --danger: #ef4444;
            --success: #22c55e;
            --table: #1e3a8a;
        }

        * { margin: 0; padding: 0; box-sizing: border-box; font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; }
        body { background-color: var(--bg-color); color: var(--text); min-height: 100vh; display: flex; flex-direction: column; }

        header {
            background-color: var(--card-bg); padding: 15px 20px;
            display: flex; justify-content: space-between; align-items: center;
            border-bottom: 2px solid #334155;
        }
        .logo { font-size: 1.5rem; font-weight: 800; color: var(--accent); }
        .balance-display { background: #0f172a; padding: 8px 15px; border-radius: 20px; font-weight: bold; border: 1px solid var(--accent); color: var(--accent); }
        .btn-back { background: #334155; color: white; text-decoration: none; padding: 8px 15px; border-radius: 5px; font-weight: bold; }

        main { flex-grow: 1; display: flex; flex-direction: column; align-items: center; padding: 20px; }
        
        .table-area {
            background: #064e3b; border: 10px solid #78350f;
            border-radius: 20px; padding: 20px; width: 100%; max-width: 900px;
            min-height: 500px; display: flex; flex-direction: column; justify-content: space-between;
            box-shadow: inset 0 0 50px rgba(0,0,0,0.5); position: relative;
        }

        /* Козырь и Колода */
        .deck-area { position: absolute; top: 20px; left: 20px; display: flex; align-items: center; gap: 15px; }
        .deck-card { width: 60px; height: 90px; background: repeating-linear-gradient(45deg, #1e3a8a, #1e3a8a 10px, #172554 10px, #172554 20px); border-radius: 5px; border: 2px solid white; }
        .trump-display { background: white; padding: 5px 10px; border-radius: 5px; color: black; font-weight: bold; font-size: 1.2rem; display: flex; align-items: center; gap: 5px; }
        .trump-red { color: #ef4444; }
        .trump-black { color: #0f172a; }

        /* Игровое поле (куда кидают карты) */
        .battlefield { display: flex; flex-wrap: wrap; gap: 15px; justify-content: center; align-items: center; min-height: 150px; margin: 40px 0; }
        
        .pair { position: relative; width: 70px; height: 100px; }
        .pair .card { position: absolute; top: 0; left: 0; }
        .pair .card.beat { top: 15px; left: 15px; box-shadow: none; }

        /* Руки */
        .hand-area { display: flex; flex-wrap: wrap; gap: 5px; justify-content: center; min-height: 100px; }
        .card {
            width: 70px; height: 100px; background: white; border-radius: 8px;
            display: flex; justify-content: center; align-items: center;
            font-size: 1.5rem; font-weight: bold; box-shadow: 0 4px 8px rgba(0,0,0,0.3);
            transition: transform 0.2s; user-select: none;
        }
        .card.red { color: #ef4444; }
        .card.black { color: #0f172a; }
        .card.clickable { cursor: pointer; }
        .card.clickable:hover { transform: translateY(-15px); }
        
        .controls { text-align: center; margin-top: 20px; }
        .btn { padding: 12px 25px; font-size: 1.1rem; font-weight: bold; border: none; border-radius: 8px; cursor: pointer; margin: 5px; }
        .btn-primary { background: var(--accent); color: #000; }
        .btn-danger { background: var(--danger); color: white; }
        
        .message { font-size: 1.4rem; font-weight: bold; text-align: center; margin: 15px 0; color: var(--accent); text-shadow: 2px 2px 4px black; }
        .hide { display: none !important; }
        
        .player-label { font-size: 1rem; margin-bottom: 5px; font-weight: bold; opacity: 0.8; text-align: center; }
    </style>
</head>
<body>

    <header>
        <a href="index.html" class="btn-back">← В лобби</a>
        <div class="logo">♥️ Дурак</div>
        <div class="balance-display">💰 <span id="balance-val">...</span></div>
    </header>

    <main>
        <div class="table-area">
            
            <div class="deck-area">
                <div class="deck-card" id="deck-pile"></div>
                <div class="trump-display" id="trump-info">
                    Козырь: <span id="trump-suit">?</span>
                </div>
            </div>

            <div>
                <div class="player-label">Рука ИИ (<span id="ai-count">0</span> карт)</div>
                <div class="hand-area" id="ai-hand"></div>
            </div>

            <div class="battlefield" id="battlefield"></div>
            <div class="message" id="game-message"></div>

            <div>
                <div class="hand-area" id="player-hand"></div>
                <div class="player-label">Твоя рука (<span id="player-count">0</span> карт)</div>
            </div>

        </div>

        <div class="controls" id="controls">
            <button class="btn btn-primary" id="btn-start" onclick="startGame()">Начать игру (Ставка 5000)</button>
            <div id="game-buttons" class="hide">
                <button class="btn btn-primary" onclick="takeCards()">Взять (Бито)</button>
            </div>
        </div>
    </main>

    <script>
        // --- БАЗА ---
        let balance = parseInt(localStorage.getItem('casinoBalance')) || 100000;
        document.getElementById('balance-val').innerText = balance.toLocaleString('ru-RU');
        function updateBalance() {
            localStorage.setItem('casinoBalance', balance);
            document.getElementById('balance-val').innerText = balance.toLocaleString('ru-RU');
        }

        // --- КАРТЫ ---
        const suits = ['♠', '♣', '♥', '♦'];
        const values = ['6', '7', '8', '9', '10', 'J', 'Q', 'K', 'A'];
        let deck = [];
        let trumpSuit = '';
        let playerHand = [];
        let aiHand = [];
        let table = []; // Карты на столе { attack: card, defend: card or null }
        let isPlayerTurn = true; // Кто атакует
        let gameActive = false;

        function createDeck() {
            deck = [];
            for (let s of suits) {
                for (let v of values) {
                    deck.push({ suit: s, value: v });
                }
            }
            // Перемешиваем
            for (let i = deck.length - 1; i > 0; i--) {
                const j = Math.floor(Math.random() * (i + 1));
                [deck[i], deck[j]] = [deck[j], deck[i]];
            }
            trumpSuit = suits[Math.floor(Math.random() * 4)];
        }

        function getCardValue(card) {
            if (card.value === 'J') return 11;
            if (card.value === 'Q') return 12;
            if (card.value === 'K') return 13;
            if (card.value === 'A') return 14;
            return parseInt(card.value);
        }

        function canBeat(defendCard, attackCard) {
            if (defendCard.suit === attackCard.suit && getCardValue(defendCard) > getCardValue(attackCard)) return true;
            if (defendCard.suit === trumpSuit && attackCard.suit !== trumpSuit) return true;
            return false;
        }

        function renderCard(card, isClickable = false, onClick = null) {
            const div = document.createElement('div');
            div.className = `card ${(card.suit === '♥' || card.suit === '♦') ? 'red' : 'black'}`;
            if (isClickable) div.classList.add('clickable');
            div.innerText = card.value + card.suit;
            if (isClickable && onClick) div.onclick = onClick;
            return div;
        }

        function startGame() {
            if (balance < 5000) return alert("Недостаточно денег для ставки!");
            balance -= 5000;
            updateBalance();

            gameActive = true;
            document.getElementById('btn-start').classList.add('hide');
            document.getElementById('game-buttons').classList.remove('hide');
            document.getElementById('game-message').innerText = "Вы атакуете! Выберите карту.";

            createDeck();
            
            // Показываем козырь
            const trumpEl = document.getElementById('trump-suit');
            trumpEl.innerText = trumpSuit;
            trumpEl.className = (trumpSuit === '♥' || trumpSuit === '♦') ? 'trump-red' : 'trump-black';

            // Раздаем по 6 карт
            playerHand = deck.splice(0, 6);
            aiHand = deck.splice(0, 6);
            
            table = [];
            isPlayerTurn = true;
            updateUI();
        }

        function updateUI() {
            // Отрисовка ИИ (рубашкой вверх)
            const aiHandEl = document.getElementById('ai-hand');
            aiHandEl.innerHTML = '';
            document.getElementById('ai-count').innerText = aiHand.length;
            for (let i = 0; i < aiHand.length; i++) {
                const cardBack = document.createElement('div');
                cardBack.className = 'card';
                cardBack.style.background = 'repeating-linear-gradient(45deg, #1e3a8a, #1e3a8a 10px, #172554 10px, #172554 20px)';
                cardBack.style.border = '2px solid white';
                aiHandEl.appendChild(cardBack);
            }

            // Отрисовка стола
            const bf = document.getElementById('battlefield');
            bf.innerHTML = '';
            table.forEach(pair => {
                const pairDiv = document.createElement('div');
                pairDiv.className = 'pair';
                const atkCard = renderCard(pair.attack);
                pairDiv.appendChild(atkCard);
                if (pair.defend) {
                    const defCard = renderCard(pair.defend);
                    defCard.classList.add('beat');
                    pairDiv.appendChild(defCard);
                }
                bf.appendChild(pairDiv);
            });

            // Отрисовка игрока
            const playerHandEl = document.getElementById('player-hand');
            playerHandEl.innerHTML = '';
            document.getElementById('player-count').innerText = playerHand.length;
            
            // Сортируем руку для удобства (сначала козыри, потом по рангу)
            playerHand.sort((a, b) => {
                if (a.suit === trumpSuit && b.suit !== trumpSuit) return 1;
                if (b.suit === trumpSuit && a.suit !== trumpSuit) return -1;
                return getCardValue(a) - getCardValue(b);
            });

            playerHand.forEach((card, index) => {
                const isClickable = isPlayerTurn && gameActive;
                const cardEl = renderCard(card, isClickable, () => playerAction(index));
                playerHandEl.appendChild(cardEl);
            });
        }

        function playerAction(index) {
            if (!isPlayerTurn || !gameActive) return;
            const card = playerHand[index];

            // Если стол пустой - это атака
            if (table.length === 0) {
                table.push({ attack: card, defend: null });
                playerHand.splice(index, 1);
                isPlayerTurn = false;
                updateUI();
                document.getElementById('game-message').innerText = "ИИ думает...";
                setTimeout(aiTurn, 1000);
                return;
            }

            // Если есть неотбитые карты - это защита
            let undefendedPair = table.find(p => !p.defend);
            if (undefendedPair) {
                if (canBeat(card, undefendedPair.attack)) {
                    undefendedPair.defend = card;
                    playerHand.splice(index, 1);
                    updateUI();
                    
                    // Если все отбито, ИИ может подкинуть или сказать "Бито"
                    if (table.every(p => p.defend)) {
                        document.getElementById('game-message').innerText = "ИИ думает...";
                        setTimeout(aiTurn, 1000);
                    }
                } else {
                    alert("Эта карта не бьет!");
                }
            }
        }

        function takeCards() {
            if (!isPlayerTurn) return; // Взять можно только когда отбиваешься
            
            // Игрок забирает все карты со стола
            table.forEach(pair => {
                playerHand.push(pair.attack);
                if (pair.defend) playerHand.push(pair.defend);
            });
            table = [];
            
            // Добираем до 6
            drawCards(playerHand);
            
            isPlayerTurn = false;
            updateUI();
            document.getElementById('game-message').innerText = "Вы взяли карты. ИИ атакует...";
            setTimeout(aiTurn, 1000);
        }

        function drawCards(hand) {
            while (hand.length < 6 && deck.length > 0) {
                hand.push(deck.pop());
            }
        }

        function aiTurn() {
            if (!gameActive) return;

            if (isPlayerTurn === false) {
                // ИИ либо отбивается, либо берет
                let undefended = table.find(p => !p.defend);
                if (undefended) {
                    // Ищем карту для защиты
                    let defendCardIndex = aiHand.findIndex(c => canBeat(c, undefended.attack));
                    if (defendCardIndex !== -1) {
                        undefended.defend = aiHand[defendCardIndex];
                        aiHand.splice(defendCardIndex, 1);
                        updateUI();
                        setTimeout(aiTurn, 1000); // Продолжаем отбиваться, если есть еще
                    } else {
                        // Не может отбиться - забирает
                        table.forEach(p => {
                            aiHand.push(p.attack);
                            if (p.defend) aiHand.push(p.defend);
                        });
                        table = [];
                        drawCards(aiHand);
                        isPlayerTurn = true;
                        updateUI();
                        document.getElementById('game-message').innerText = "ИИ взял карты. Ваш ход (атака).";
                        checkWin();
                    }
                } else {
                    // Все отбито. Может ли ИИ подкинуть?
                    let canAdd = aiHand.find(c => table.some(p => p.attack.value === c.value));
                    if (canAdd && table.length < 6) {
                        table.push({ attack: canAdd, defend: null });
                        aiHand.splice(aiHand.indexOf(canAdd), 1);
                        updateUI();
                        document.getElementById('game-message').innerText = "ИИ подкинул карту. Отбивайтесь!";
                        isPlayerTurn = true; // Теперь игрок должен отбиться
                        updateUI();
                    } else {
                        // ИИ говорит "Бито"
                        table = [];
                        isPlayerTurn = true;
                        drawCards(playerHand);
                        drawCards(aiHand);
                        updateUI();
                        document.getElementById('game-message').innerText = "Бито! Ваш ход.";
                        checkWin();
                    }
                }
            } else {
                // ИИ атакует
                if (table.length === 0) {
                    // ИИ ходит первой картой
                    if (aiHand.length > 0) {
                        let card = aiHand.pop();
                        table.push({ attack: card, defend: null });
                        isPlayerTurn = true; 
                        updateUI();
                        document.getElementById('game-message').innerText = "ИИ атакует. Отбивайтесь!";
                    }
                } else {
                    // Игрок отбился, ИИ может подкинуть
                    let canAdd = aiHand.find(c => table.some(p => p.attack.value === c.value));
                    if (canAdd && table.length < 6) {
                        table.push({ attack: canAdd, defend: null });
                        aiHand.splice(aiHand.indexOf(canAdd), 1);
                        isPlayerTurn = true;
                        updateUI();
                        document.getElementById('game-message').innerText = "ИИ подкинул. Отбивайтесь!";
                    } else {
                        table = [];
                        isPlayerTurn = true;
                        drawCards(playerHand);
                        drawCards(aiHand);
                        updateUI();
                        document.getElementById('game-message').innerText = "Бито! Вы атакуете.";
                        checkWin();
                    }
                }
            }
        }

        function checkWin() {
            if (playerHand.length === 0) {
                gameActive = false;
                balance += 10000;
                updateBalance();
                document.getElementById('game-message').innerText = "🎉 ВЫ ВЫИГРАЛИ! +10 000 монет.";
                document.getElementById('game-buttons').classList.add('hide');
                document.getElementById('btn-start').classList.remove('hide');
                document.getElementById('btn-start').innerText = "Играть снова (Ставка 5000)";
            } else if (aiHand.length === 0) {
                gameActive = false;
                document.getElementById('game-message').innerText = "😢 ВЫ ПРОИГРАЛИ. -5 000 монет.";
                document.getElementById('game-buttons').classList.add('hide');
                document.getElementById('btn-start').classList.remove('hide');
                document.getElementById('btn-start').innerText = "Попробовать снова (Ставка 5000)";
            }
        }
    </script>
</body>
</html>